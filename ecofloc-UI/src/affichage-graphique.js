/*
let precedentTimeStamp = 0;
let totalW = 0;
let mainIdGraph = "";

const dictionnaireComposants = {
  "CPU":"CPU",
  "GPU":"GPU",
  "RAM":"RAM",
  "NIC":"NIC",
  "SD":"SD",
  "TOTAL":"Total Consommation"
};

const unGraph = (id, value) => {
  return `
  <div id="box${id}" class="bg-zinc-800 p-4 rounded-md h-full">
    <h3 class="text-center font-semibold">${value}</h3>
    <div id="graph${id}" class="responsive-graph flex-grow"></div>
  </div>`
}

const initialGraph = () => {
  let content = `<div id="containerBox" class="grid grid-cols-3 grid-rows-2 gap-4 max-h-[60vh] h-full">`;
  for (let cle in dictionnaireComposants) {
    content += unGraph(cle, dictionnaireComposants[cle]);
  } 
  content += `</div>`;
  return content;
}

const detailedMarkup = (id) => {
  let content = `
  <div id="containerBox" class="max-h-[60vh] h-full flex">
    <div class="h-full" style="width: 80%; margin-right: 30px;">` +
      unGraph(id, dictionnaireComposants[id])
    + `</div>
    <div class="h-full" style="flex-grow: 1;">
  ` 
  for (let cle in dictionnaireComposants) {
    if(id != cle) {
      content += `<div style="height: 20%;">`;
      content +=  unGraph(cle, dictionnaireComposants[cle]);
      content += `</div>`;
    } 
  }
    content += `
        </div>
      </div>
    `
    return content;
};

function setListener(){
  for (let cle in dictionnaireGraphComposants) {
    if(cle == "TOTAL") {
      continue;
    }
    document.getElementById(`checkbox`+cle).addEventListener("click", (event) => {
      dictionnaireGraphComposants[cle].show(event.target.checked);
    });
  }
  for (let cle in dictionnaireComposants) {
    document.getElementById(`box`+cle).addEventListener("click", () => {
    showDetailView(cle);
  });
  }
}

function mettreAJourTousLesElement(){
  graphCPU.mettreAJourElement();
  graphGPU.mettreAJourElement();
  graphNIC.mettreAJourElement();
  graphRAM.mettreAJourElement();
  graphSD.mettreAJourElement();
  graphTOTAL.mettreAJourElement();
}


function renderInitialView() {
  document.getElementById("flex-graphique").innerHTML = initialGraph();
}

function showDetailView(id) {
  if(this.mainIdGraph == id) {
    renderInitialView();
    this.mainIdGraph = "";
  }
  else {
    document.getElementById("flex-graphique").innerHTML = detailedMarkup(id);
    this.mainIdGraph = id;
  }
  mettreAJourTousLesElement();
  setListener();
}

function readFile() {
    fetch('./system_monitoring.json')
    .then(response => {
      if (response.ok) {
        return response.text(); // Get raw text, not JSON yet
      }
      return null;  // Return null if the response isn't OK
    })
    .then(text => {
      try {
        const data = text ? JSON.parse(text) : null; // Try to parse JSON, or return null if empty
        if (data) {
          handleJSON(data);  // Only process the data if it's valid
        }
      } catch (error) {
        // Ignore any errors related to JSON parsing
      }
    });
}  

function handleJSON(data){
    if(data['time'] != precedentTimeStamp) {
        precedentTimeStamp = data['time'];
        updatePlot(data);
    }
} 

function updatePlot(data) {
    const datacomponents = data['apps'][0]['components'];
    totalW = 0;
    graphGPU.updatePlot(100);
    graphRAM.updatePlot(100);
    for(let c of datacomponents){
      
      switch (c['type']){
        case 'CPU':
          graphCPU.updatePlot(c['power_w']);
          totalW += c['power_w'];
          break;
        case 'NIC':
          graphNIC.updatePlot(c['power_w']);
          totalW += c['power_w'];
          break;
        case 'SD':
          graphSD.updatePlot(c['power_w']);
          totalW += c['power_w'];
          break;
        default:
          console.log(`Sorry, we are out of ${c['type']}.`);
          break;
      }
    }
    graphTOTAL.updatePlot(totalW);
}

renderInitialView();
let graphCPU = new DynamicGraph("graphCPU", "rgb(248 113 113 / var(--tw-bg-opacity, 1))");
let graphSD = new DynamicGraph("graphSD", "rgb(129 140 248 / var(--tw-bg-opacity, 1))");
let graphNIC = new DynamicGraph("graphNIC", "rgb(96 165 250 / var(--tw-bg-opacity, 1))");
let graphGPU = new DynamicGraph("graphGPU", "rgb(74 222 128 / var(--tw-bg-opacity, 1))");
let graphRAM = new DynamicGraph("graphRAM", "rgb(45 212 191 / var(--tw-bg-opacity, 1))");
let graphTOTAL = new DynamicGraph("graphTOTAL",  "rgb(192 132 252 / var(--tw-bg-opacity, 1))");
const dictionnaireGraphComposants = {
  "CPU":graphCPU,
  "GPU":graphGPU,
  "RAM":graphRAM,
  "NIC":graphNIC,
  "SD":graphSD,
  "TOTAL":graphTOTAL
};
setListener();
setInterval(readFile, 500);*/