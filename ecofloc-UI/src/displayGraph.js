let previousTimeStamp = 0;
let totalW = 0;
/*const list_tab = document.getElementById("list-tab");
const flex_graph = document.getElementById("flex-graph");*/

function setListener(){
  for (let cle in dictionaryGraphComponents) {
    if(cle == "TOTAL") {
      continue;
    }
    document.getElementById(`checkbox`+cle).addEventListener("click", (event) => {
      dictionaryGraphComponents[cle].show(event.target.checked);
    });
  }
  let flexElements = document.querySelectorAll(".graph");
  for (let unFlexElement of flexElements) {
      unFlexElement.addEventListener("click", showDetailView);
  }
}

function updateAllElements(){
  graphCPU.updateElements();
  graphGPU.updateElements();
  graphNIC.updateElements();
  graphRAM.updateElements();
  graphSD.updateElements();
  graphTOTAL.updateElements();
}

function showDetailView(event) {
  let graph = event.target;
  while(!graph.classList.contains("graph")){
    graph = graph.parentElement;
  }
  let elementFlex = document.getElementById("flex-graph");
  if(graph.classList.contains("select")){
      elementFlex.classList.remove("detail");
      graph.classList.remove("select")
  } else{
      if(!elementFlex.classList.contains("detail")){
          elementFlex.classList.add("detail");
      }
      let flexElements = document.querySelectorAll(".graph");
      for (let unFlexElement of flexElements) {
          if(unFlexElement.classList.contains("select")){
              unFlexElement.classList.remove("select");
          }
      }
      graph.classList.add("select");
  }
  updateAllElements();
}

function readFile() {
    fetch('./systemMonitoring.json')
    .then(response => {
      if (response.ok) {
        return response.text();
      }
      return null;  
    })
    .then(text => {
      try {
        const data = text ? JSON.parse(text) : null; 
        if (data) {
          handleJSON(data); 
        }
      } catch (error) {
          console.error(error);
      }
    });
}  

function handleJSON(data){
    if(data['time'] != previousTimeStamp) {
        previousTimeStamp = data['time'];
        updatePlot(data);
    }
} 

function updatePlot(data) {
    const dataComponents = data['apps'][0]['components'];
    totalW = 0;
    graphGPU.updatePlot(100);
    graphRAM.updatePlot(100);
    for(let c of dataComponents){
      
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

let graphCPU = new DynamicGraph("graphCPU", "rgb(248 113 113 / var(--tw-bg-opacity, 1))");
let graphSD = new DynamicGraph("graphSD", "rgb(129 140 248 / var(--tw-bg-opacity, 1))");
let graphNIC = new DynamicGraph("graphNIC", "rgb(96 165 250 / var(--tw-bg-opacity, 1))");
let graphGPU = new DynamicGraph("graphGPU", "rgb(74 222 128 / var(--tw-bg-opacity, 1))");
let graphRAM = new DynamicGraph("graphRAM", "rgb(45 212 191 / var(--tw-bg-opacity, 1))");
let graphTOTAL = new DynamicGraph("graphTOTAL",  "rgb(192 132 252 / var(--tw-bg-opacity, 1))");
const dictionaryGraphComponents = {
  "CPU":graphCPU,
  "GPU":graphGPU,
  "RAM":graphRAM,
  "NIC":graphNIC,
  "SD":graphSD,
  "TOTAL":graphTOTAL
};
setListener();
setInterval(readFile, 500);