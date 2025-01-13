const listProcessHtmlElement = document.getElementById("ListeProcessus");
const tableFilterHtmlElement = document.getElementById("TableFilter");
const checkBoxSelectAllProcElement = document.getElementById("SelectAllProc");

let myProcesses = [];
let setCategory = new Set();
function areSetsEqual(set1, set2) {
    if (set1.size !== set2.size) {
        return false; 
    }

    // Comparer chaque élément dans set1
    for (let item of set1) {
        if (!set2.has(item)) {
            return false; 
        }
    }

    return true;
}

function makeGroupApplication(){
    if(tableFilterHtmlElement) {
        tableFilterHtmlElement.innerHTML = '';
        for (const item of setCategory) {
            const lineDiv = document.createElement('div');
            lineDiv.classList.add('line', 'col-3');

            const labelDiv = document.createElement('div');
            labelDiv.textContent = item;

            const input = document.createElement('input');
            input.type = 'checkbox';
            input.id = item+'Filter';
            input.value = item;
            input.checked = true;

            lineDiv.appendChild(labelDiv);
            lineDiv.appendChild(input);

            tableFilterHtmlElement.appendChild(lineDiv);
        }
        for(let filter of tableFilterHtmlElement.querySelectorAll("input")){
            filter.checked = true;
            filter.indeterminate = false;
            filter.addEventListener("change", (event) => {
                const currentFilter = event.target;
                if(currentFilter.value == "All"){
                    for(let filter of tableFilterHtmlElement.querySelectorAll("input")) {
                        filter.checked = currentFilter.checked;
                    }
                }
                else{
                    let all = true;
                    let atLeastOne = false;
                    for(let filter of tableFilterHtmlElement.querySelectorAll("input")) {
                        if(filter.checked && filter.value != "All"){
                            atLeastOne = true;
                        }
                        if(!filter.checked && filter.value != "All"){
                            all = false;
                        }
                    }
                    const allFilterElement = document.getElementById("AllFilter");
                    allFilterElement.checked = atLeastOne;
                    allFilterElement.indeterminate = (!all && atLeastOne); 
                }
                showProcessList();
            });
        }
    }
}

function parseDataToMyProcesses(data){
    if(data){
        myProcesses = [];
        const oldCategorie = setCategory;
        setCategory = new Set();
        setCategory.add("All");
        setCategory.add("Other");
        data.forEach(process => {
            let uneApplication = new Application(process.name,process.pid, process.categorie, process.color);
            myProcesses.push(uneApplication);
            if(uneApplication.categorie != "") {
                setCategory.add(uneApplication.categorie);
            }
        });
        if(!areSetsEqual(oldCategorie, setCategory)) {
            makeGroupApplication();
        }
        showProcessList();
    }
}

fetch('process.json')
.then(response => {
    // Vérifier si la réponse est correcte
    if (!response.ok) {
        throw new Error('Error loading JSON file');
    }
    return response.json();  // Parse le JSON
})
.then(data => {

    // Afficher chaque processus dans la liste
    parseDataToMyProcesses(data);
})
.catch(error => {
    console.error('Error:', error);
});

function getFilterCategory(nomCategory) {
    for(let filter of tableFilterHtmlElement.querySelectorAll("input")){
        if(filter.value == nomCategory){
            return filter.checked;
        }
    }
    const otherFilterElement = document.getElementById("OtherFilter");
    return otherFilterElement.checked;
}

function changePidState(nomProc, pidProc, etat) {
    // Adresse de votre serveur Node.js
    const serverUrl = 'http://localhost:3030/changePidState';
    // Envoi d'une requête POST au serveur
    fetch(serverUrl, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        // Ajoutez les données
        body: JSON.stringify({ nomProc, pidProc, etat }),
    })
        .then(response => response.json())
        .then(data => {
            if (!data.success) {
                console.error('Error during execution :', data.message);
                alert(`Error : ${data.message}`);
            }
        })
        .catch(error => {
            console.error('Network or server error :', error);
            alert('Unable to contact server.');
        });
}



function showProcessList() {
    while (listProcessHtmlElement.firstChild) {
        listProcessHtmlElement.removeChild(listProcessHtmlElement.firstChild);
    }
    const searchText = document.getElementById("SearchBar").value == "" ? "":document.getElementById("SearchBar").value.toLowerCase();
    let atLeastOneChecked = false;
    let allChecked = true;
    for(let unProcessus of myProcesses){
        if (!unProcessus.getName().toLowerCase().includes(searchText)){
            continue;
        }
        if(getFilterCategory(unProcessus.categorie)){
            for(let unPid of unProcessus.getListePid()){
                // Créer les éléments
                const lineDiv = document.createElement("div");
                lineDiv.className = "line";

                const col1Div = document.createElement("div");
                col1Div.className = "col-1 cell";
                col1Div.textContent = unProcessus.getName();

                const col2Div = document.createElement("div");
                col2Div.className = "col-2 cell";

                const inputCheckbox = document.createElement("input");
                inputCheckbox.type = "checkbox";
                col2Div.textContent = unPid["numeroPid"];
                if(unPid["checked"]){
                    atLeastOneChecked = true;
                }else{
                    allChecked = false;
                }
                inputCheckbox.checked = unPid["checked"];
                inputCheckbox.setAttribute("data-nom-processus", unProcessus.getName());
                inputCheckbox.setAttribute("data-numero-pid", unPid["numeroPid"]);
                inputCheckbox.addEventListener('click', function(event) {
                    let clickedCheckbox = event.target;
                    const dataNomProcessus = event.target.getAttribute("data-nom-processus");
                    const dataNumeroPid = event.target.getAttribute("data-numero-pid");
                    changePidState(dataNomProcessus, dataNumeroPid,clickedCheckbox.checked);
                });
                col2Div.appendChild(inputCheckbox); // Ajouter la case à cocher à col-2

                // Ajouter les colonnes à la ligne
                lineDiv.appendChild(col1Div);
                lineDiv.appendChild(col2Div);

                // Ajouter la ligne à l'élément parent
                listProcessHtmlElement.appendChild(lineDiv);
            }
        }
    }
    console.log(atLeastOneChecked);
    console.log(allChecked);
    checkBoxSelectAllProcElement.checked = atLeastOneChecked;
    checkBoxSelectAllProcElement.indeterminate = (!allChecked && atLeastOneChecked); 
}

function selectAllPid(etat) {
    const searchText = document.getElementById("SearchBar").value == "" ? "":document.getElementById("SearchBar").value.toLowerCase();
    let listPidToChange = new Set();
    for(let aProcess of myProcesses){
        if (!aProcess.getName().toLowerCase().includes(searchText)){
            continue;
        }
        if(!getFilterCategory(aProcess.categorie)){
            continue;
        }
        for(let unPid of aProcess.getListePid()){
            if(unPid.checked != etat){
                listPidToChange.add(aProcess.getName());
            }
        }
    }
    console.log(listPidToChange);
    const serverUrl = 'http://localhost:3030/changeListePidState';
    // Envoi au serveur avec fetch
    fetch(serverUrl, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json', // Assurez-vous d'envoyer du JSON
        },
        body: JSON.stringify({ liste: Array.from(listPidToChange), etat: etat }), // Sérialise la liste dans le corps de la requête
    })
    .then(response => response.json()) // Récupère la réponse du serveur
    .catch(error => {
        console.error('Error sending data :', error);
    });

}

checkBoxSelectAllProcElement.addEventListener("change", (event) => {
    selectAllPid(event.target.checked);
});

const eventSource = new EventSource('http://localhost:3030/events');

eventSource.onmessage = (event) => {
    try {
        const data = JSON.parse(event.data);
        if(!data["message"]){
            parseDataToMyProcesses(data);
        }
    } catch (err) {
        console.error('Data parsing error:', err);
    }
};

eventSource.onerror = () => {
    console.error('SSE server connection error');
};

document.getElementById("SearchBar").addEventListener("keyup", () => {
    const searchText = document.getElementById("SearchBar").value;
    console.log(searchText);
    showProcessList();
});