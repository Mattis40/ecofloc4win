#define NOMINMAX

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>
#include <windows.h>
#include <mutex>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "gpu.h"
#include "MonitoringData.h"

using namespace ftxui;

std::vector<MonitoringData> monitoringData = {};
std::mutex data_mutex;

// Function to get terminal size
int GetTerminalHeight() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		// Calculate the height of the terminal window.
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}
	// Default to 24 rows if size cannot be determined.
	return 24;
}

auto CreateTableRows() -> std::vector<std::vector<std::string>> {
	std::vector<std::vector<std::string>> rows;
	std::lock_guard<std::mutex> lock(data_mutex);

	rows.emplace_back(std::vector<std::string>{"Application Name", "CPU", "GPU", "SD", "NIC"});
	for (const auto& data : monitoringData) {
		rows.emplace_back(std::vector<std::string>{
			data.getName(),
				std::to_string(data.cpuEnergy),
				std::to_string(data.gpuEnergy),
				std::to_string(data.sdEnergy),
				std::to_string(data.nicEnergy)
		});
	}

	return rows;
}

auto RenderTable(int scroll_position) -> Element {
	auto table_data = CreateTableRows();
	int terminal_height = GetTerminalHeight();
	int visible_rows = terminal_height - 8; // Adjust for input box and borders

	// Prepare rows for the visible portion
	std::vector<std::vector<std::string>> visible_table_data;
	visible_table_data.push_back(table_data[0]); // Header row
	for (int i = scroll_position + 1;
		i < std::min(scroll_position + visible_rows + 1, (int)table_data.size());
		++i) {
		visible_table_data.push_back(table_data[i]);
	}

	auto table = Table(visible_table_data);

	// Style the table
	table.SelectAll().Border(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).DecorateCells(center);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Border();
	table.SelectColumn(0).Decorate(flex);
	table.SelectColumns(0, -1).SeparatorVertical(LIGHT);
	auto content = table.SelectRows(1, -1);
	content.DecorateCellsAlternateRow(color(Color::Red), 3, 0);
	content.DecorateCellsAlternateRow(color(Color::RedLight), 3, 1);
	content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

	return table.Render() | flex;
}

int main()
{

	std::string input;
	auto input_box = Input(&input, "Type /help");

	auto screen = ScreenInteractive::Fullscreen();


	// State variables for scrolling
	int scroll_position = 0;

	// Component to handle input and update the scroll position
	auto component = Renderer(input_box, [&] {
		return vbox({
			RenderTable(scroll_position),
			separator(),
			hbox({
				text("Command: "), input_box->Render()
			}),
			}) | border;
		});

	component = CatchEvent(component, [&](Event event) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8;

		if ((int)monitoringData.size() <= visible_rows) {
			scroll_position = 0; // Disable scrolling if all rows fit
			return false;
		}

		// Handle mouse wheel and arrow key events
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::WheelDown) {
				scroll_position = std::min(scroll_position + 1, (int)monitoringData.size() - visible_rows - 1);
				return true;
			}
			if (event.mouse().button == Mouse::WheelUp) {
				scroll_position = std::max(scroll_position - 1, 0);
				return true;
			}
		}

		if (event == Event::ArrowDown) {
			scroll_position = std::min(scroll_position + 1, (int)monitoringData.size() - visible_rows - 1);
			return true;
		}
		if (event == Event::ArrowUp) {
			scroll_position = std::max(scroll_position - 1, 0);
			return true;
		}

		return false;
		});

	MonitoringData blender("Blender", { 27232 });
	{
		std::lock_guard<std::mutex> lock(data_mutex);
		monitoringData.push_back(blender);
	}
	
	std::thread gpu_thread([&screen] {
		while (true) {
			{
				std::lock_guard<std::mutex> lock(data_mutex);
				for (auto& data : monitoringData)
				{
					std::vector<int> gpu_usage = GPU::getGPUUsage(data.getPids());
					int gpu_power = GPU::getGPUPower();
					data.updateGPUEnergy(gpu_power);
				}
			}
			screen.Post(Event::Custom);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	std::thread redraw_thread([&screen] {
		while (true) {
			screen.PostEvent(Event::Custom);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});

	// Run the application
	screen.Loop(component);

	redraw_thread.join();
	gpu_thread.join();
	return 0;
}