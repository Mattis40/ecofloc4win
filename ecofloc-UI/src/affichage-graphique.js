let precedentTimeStamp = 0;
let totalW = 0;
let mainIdGraph = "";
const listTab = document.getElementById("list-tab");
const flexGraphique = document.getElementById("flex-graphique");

const componentDictionary = {
    "CPU": "CPU",
    "GPU": "GPU",
    "NIC": "NIC",
    "SD": "SD",
    "TOTAL": "Total Consumption",
    "RAM": "RAM"
};

// Generate a graph HTML structure
const generateGraphHTML = (id, label) => {
    if (label === "RAM") {
        return `
            <div class="graphique">
                <h3 class="text-center font-semibold">RAM</h3>
                <div class="flex flex-grow justify-center items-center h-[90%] bg-wip rounded-md">
                    <p class="text-center font-bold bg-zinc-800 p-4 rounded-md fixed wip">Work in progress</p>
                </div>
            </div>`;
    }
    return `
        <div id="box${id}" class="graphique">
            <h3 class="text-center font-semibold">${label}</h3>
            <div id="graph${id}" class="flex h-full"></div>
        </div>`;
};

// Initialize all graphs
const initializeGraphs = () => {
    return Object.keys(componentDictionary).map(key => generateGraphHTML(key, componentDictionary[key])).join('');
};

// Generate markup for detailed view
const generateDetailedMarkup = (id) => {
    let content = `<div class="h-full" style="width: 80%; margin-right: 30px;">${generateGraphHTML(id, componentDictionary[id])}</div>
                   <div class="h-full flex flex-col gap-1" style="flex-grow: 1;">`;
    for (let key in componentDictionary) {
        if (id !== key) {
            content += generateGraphHTML(key, componentDictionary[key]);
        }
    }
    return content + `</div>`;
};

// Update all graphs
const refreshAllGraphs = () => {
    graphCPU.refreshGraph();
    graphGPU.refreshGraph();
    graphNIC.refreshGraph();
    graphSD.refreshGraph();
    graphTOTAL.refreshGraph();
};

// Render the initial graph view
const renderInitialView = () => {
    flexGraphique.innerHTML = initializeGraphs();
};

// Read the JSON file and update plots
const readFile = () => {
    fetch('./system_monitoring.json')
        .then(response => response.ok ? response.json() : null)
        .then(data => {
            if (data && data.time !== precedentTimeStamp) {
                precedentTimeStamp = data.time;
                updatePlots(data);
            }
        })
        .catch(error => console.error("Error reading file:", error));
};

// Update individual plots based on the JSON data
const updatePlots = (data) => {
    const components = data.apps[0].components;
    totalW = 0;

    components.forEach(component => {
        const power = component.power_w || 0;
        switch (component.type) {
            case 'CPU':
                graphCPU.updatePlot(power);
                totalW += power;
                break;
            case 'NIC':
                graphNIC.updatePlot(power);
                totalW += power;
                break;
            case 'GPU':
                graphGPU.updatePlot(power);
                totalW += power;
                break;
            case 'SD':
              graphSD.updatePlot(power);
              totalW += power;
              break;
            default:
                console.warn(`Unknown component type: ${component.type}`);
        }
    });

    graphTOTAL.updatePlot(totalW);
};

// Initialize graphs and set periodic updates
renderInitialView();
let graphCPU = new DynamicGraph("graphCPU", "rgb(248, 113, 113)");
let graphGPU = new DynamicGraph("graphGPU", "rgb(74, 222, 128)");
let graphNIC = new DynamicGraph("graphNIC", "rgb(96, 165, 250)");
let graphSD = new DynamicGraph("graphSD", "rgb(129, 140, 248)");
let graphTOTAL = new DynamicGraph("graphTOTAL", "rgb(192, 132, 252)");

setInterval(readFile, 500);