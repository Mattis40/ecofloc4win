let precedentTimeStamp = 0;
let totalW = 0;
let mainIdGraph = "";
const list_tab = document.getElementById("list-tab");
const flex_graphique = document.getElementById("flex-graphique");

const dictionnaireComposants = {
  "CPU":"CPU",
  "GPU":"GPU",
  "NIC":"NIC",
  "SD":"SD",
  "TOTAL":"Total Consommation",
  "RAM":"RAM"
};

const unGraph = (id, value) => {

  if (value == "RAM") {
    return `
        <div class="graphique">
          <h3 class="text-center font-semibold">RAM</h3>
          <div class="flex flex-grow justify-center items-center h-[90%] bg-wip rounded-md" >
            <p class="text-center font-bold bg-zinc-800 p-4 rounded-md fixed">Work in progress...</p>
            </div>
        </div>`
  }
  return `
  <div id="box${id}" class="graphique">
    <h3 class="text-center font-semibold">${value}</h3>
    <div id="graph${id}" class="flex h-full"></div>
  </div>`
}

const initialGraph = () => {
  let content = "";
  for (let cle in dictionnaireComposants) {
    content += unGraph(cle, dictionnaireComposants[cle]);
  } 
  return content;
}

const detailedMarkup = (id) => {
  let content = `
    <div class="h-full" style="width: 80%; margin-right: 30px;">` +
      unGraph(id, dictionnaireComposants[id])
    + `</div>
    <div class="h-full flex flex-col gap-1" style="flex-grow: 1;">
  ` 
  for (let cle in dictionnaireComposants) {
    if(id != cle) {
      content +=  unGraph(cle, dictionnaireComposants[cle]);
    } 
  }
    content += `
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
  flex_graphique.innerHTML = initialGraph();
}

function showDetailView(id) {
  if(this.mainIdGraph == id) {
    list_tab.classList.add('flex');
    list_tab.classList.remove('hide');
    flex_graphique.classList.add('h-3/4');
    flex_graphique.classList.remove('h-full');
    renderInitialView();
    this.mainIdGraph = "";
  }
  else {
    list_tab.classList.remove('flex');
    list_tab.classList.add('hide');
    flex_graphique.classList.add('h-full');
    flex_graphique.classList.remove('h-3/4');
    flex_graphique.innerHTML = detailedMarkup(id);
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
let graphTOTAL = new DynamicGraph("graphTOTAL",  "rgb(192 132 252 / var(--tw-bg-opacity, 1))");
//let graphRAM = new DynamicGraph("graphRAM", "rgb(45 212 191 / var(--tw-bg-opacity, 1))");
const dictionnaireGraphComposants = {
  "CPU":graphCPU,
  "GPU":graphGPU,
  "NIC":graphNIC,
  "SD":graphSD,
  "TOTAL":graphTOTAL,
  "RAM":graphRAM
};
setListener();
setInterval(readFile, 500);