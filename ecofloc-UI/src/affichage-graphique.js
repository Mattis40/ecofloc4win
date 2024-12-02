
let precedentTimeStamp = 0;
let totalW = 0;
let graphCPU = new DynamicGraph("graphCPU", "rgb(248 113 113 / var(--tw-bg-opacity, 1))");
let graphSD = new DynamicGraph("graphSD", "rgb(129 140 248 / var(--tw-bg-opacity, 1))");
let graphNIC = new DynamicGraph("graphNIC", "rgb(96 165 250 / var(--tw-bg-opacity, 1))");
let graphGPU = new DynamicGraph("graphGPU", "rgb(74 222 128 / var(--tw-bg-opacity, 1))");
let graphRAM = new DynamicGraph("graphRAM", "rgb(45 212 191 / var(--tw-bg-opacity, 1))");
let graphTOTAL = new DynamicGraph("graphTOTAL",  "rgb(192 132 252 / var(--tw-bg-opacity, 1))");

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
      }
    }
    graphTOTAL.updatePlot(totalW);
}
setInterval(readFile, 500);

