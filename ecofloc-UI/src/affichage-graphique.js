let precedentTimeStamp = 0;
let totalW = 0;
const list_tab = document.getElementById("list-tab");
const flex_graphique = document.getElementById("flex-graphique");

function setListener(){
  for (let cle in dictionnaireGraphComposants) {
    if(cle == "TOTAL") {
      continue;
    }
    document.getElementById(`checkbox`+cle).addEventListener("click", (event) => {
      dictionnaireGraphComposants[cle].show(event.target.checked);
    });
  }
  let flexElements = document.querySelectorAll(".graphique");
  for (let unFlexElement of flexElements) {
      unFlexElement.addEventListener("click", showDetailView);
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

function showDetailView(event) {
  let graphique = event.target;
  while(!graphique.classList.contains("graphique")){
    graphique = graphique.parentElement;
  }
  let elementFlex = document.getElementById("flex-graphique");
  if(graphique.classList.contains("selectionner")){
      elementFlex.classList.remove("detail");
      graphique.classList.remove("selectionner")
  } else{
      if(!elementFlex.classList.contains("detail")){
          elementFlex.classList.add("detail");
      }
      let flexElements = document.querySelectorAll(".graphique");
      for (let unFlexElement of flexElements) {
          if(unFlexElement.classList.contains("selectionner")){
              unFlexElement.classList.remove("selectionner");
          }
      }
      graphique.classList.add("selectionner");
  }
  mettreAJourTousLesElement();
}

function readFile() {
    fetch('./system_monitoring.json')
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
setInterval(readFile, 500);