#define NOMINMAX

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>
#include <windows.h>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;

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

int main()
{
	std::string input;
	auto input_box = Input(&input, "Type here");
	auto cell = [](const char* t) { return text(t) | border; };

	auto screen = ScreenInteractive::Fullscreen();

	std::vector<std::vector<std::string>> table_data = {
		{"Application Name", "CPU", "GPU", "SD", "NIC"},
		{"Firefox", "5120", "9852", "4563", "845"},
		{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
		//{"Firefox", "5120", "9852", "4563", "845"},
		//{"explorer.exe", "2560", "4251", "3561", "778"},
	};

	auto table = Table(table_data);

	// State variables for scrolling
	int scroll_position = 0;

	auto render_table = [&](int scroll_position) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8; // Adjust for input box and borders

		// Prepare table header
		std::vector<std::string> header = table_data[0];
		std::vector<std::vector<std::string>> rows;

		rows.push_back(header);
		// Prepare table rows
		for (int i = scroll_position + 1; i < std::min(scroll_position + visible_rows + 1, (int)table_data.size()); ++i) {
			rows.push_back(table_data[i]);
		}

		auto table = Table(rows);

		// Decorate the table
		table.SelectAll().Border(LIGHT);
		table.SelectRow(0).Decorate(bold);
		table.SelectRow(0).DecorateCells(center);
		table.SelectRow(0).SeparatorVertical(LIGHT);
		table.SelectRow(0).Border();
		table.SelectColumn(0).Decorate(flex);
		table.SelectColumns(0, -1).SeparatorVertical(LIGHT);
		auto content = table.SelectRows(1, -1);
		content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
		content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
		content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

		// Create the table with consistent column widths
		return table.Render() | flex;
		};

	// Component to handle input and update the scroll position
	auto component = Renderer(input_box, [&] {
		return vbox({
			render_table(scroll_position),
			separator(),
			hbox({
				text("Command: "),
				input_box->Render(),
				}),
			}) | border;
		});

	component = CatchEvent(component, [&](Event event) {
		int terminal_height = GetTerminalHeight();
		int visible_rows = terminal_height - 8;

		// Disable scrolling if all rows fit within the visible area
		if ((int)table_data.size() <= visible_rows) {
			scroll_position = 0;
			return false;
		}

		// Handle mouse wheel events and arrow key events
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::WheelDown) {
				scroll_position = std::min(scroll_position + 1, (int)table_data.size() - visible_rows - 1);
				return true;
			}
			if (event.mouse().button == Mouse::WheelUp) {
				scroll_position = std::max(scroll_position - 1, 0);
				return true;
			}
		}

		if (event == Event::ArrowDown) {
			scroll_position = std::min(scroll_position + 1, (int)table_data.size() - visible_rows - 1);
			return true;
		}
		if (event == Event::ArrowUp) {
			scroll_position = std::max(scroll_position - 1, 0);
			return true;
		}

		return false;
		});

	// Run the application
	screen.Loop(component);
	return 0;
}