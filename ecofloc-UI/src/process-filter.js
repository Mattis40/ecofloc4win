const listeProcessusHtmlElement = document.getElementById("ListeProcessus");
const tableFilterHtmlElement = document.getElementById("TableFilter")
let mesProcessus = [];
const otherFilterElement = document.getElementById("OtherFilter");
const allFilterElement = document.getElementById("AllFilter");

fetch('process.json')
.then(response => {
    // Vérifier si la réponse est correcte
    if (!response.ok) {
        throw new Error('Erreur de chargement du fichier JSON');
    }
    return response.json();  // Parse le JSON
})
.then(data => {

    // Afficher chaque processus dans la liste
    data.forEach(process => {
        let uneApplication = new Application(process.name,process.pid, process.categorie, process.color);
        mesProcessus.push(uneApplication);
    });
    afficherListeProcessus();
})
.catch(error => {
    console.error('Erreur:', error);
});

if(tableFilterHtmlElement){
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
                allFilterElement.checked = atLeastOne;
                allFilterElement.indeterminate = (!all && atLeastOne); 
            }
            afficherListeProcessus();
        });
    }
}

function getFilterCategorie(nomCategorie) {
    for(let filter of tableFilterHtmlElement.querySelectorAll("input")){
        if(filter.value == nomCategorie){
            return filter.checked;
        }
    }
    return otherFilterElement.checked;
}

function changePidState(nomProc, pidProc, etat) {
    // Adresse de votre serveur Node.js
    const serverUrl = 'http://localhost:3030/changePidState';
    console.log(nomProc);
    console.log(pidProc);
    console.log(etat);
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
            if (data.success) {
                console.log('Script exécuté avec succès :', data.stdout);
                //alert('Script exécuté avec succès !');
            } else {
                console.error('Erreur lors de l\'exécution :', data.message);
                alert(`Erreur : ${data.message}`);
            }
        })
        .catch(error => {
            console.error('Erreur réseau ou serveur :', error);
            alert('Impossible de contacter le serveur.');
        });
}



function afficherListeProcessus() {
    while (listeProcessusHtmlElement.firstChild) {
        listeProcessusHtmlElement.removeChild(listeProcessusHtmlElement.firstChild);
    }
    for(let unProcessus of mesProcessus){
        if(getFilterCategorie(unProcessus.categorie)){
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
                inputCheckbox.checked = unPid["checked"];
                inputCheckbox.setAttribute("data-nom-processus", unProcessus.getName());
                inputCheckbox.setAttribute("data-numero-pid", unPid["numeroPid"]);
                inputCheckbox.addEventListener('click', function(event) {
                    let clickedCheckbox = event.target;
                    const dataNomProcessus = event.target.getAttribute("data-nom-processus");
                    const dataNumeroPid = event.target.getAttribute("data-numero-pid");
                    changePidState(dataNomProcessus, dataNumeroPid,clickedCheckbox.checked);
                    if (clickedCheckbox.checked) {
                        console.log('La checkbox est cochée');
                    } else {
                        console.log('La checkbox n\'est pas cochée');
                    }
                });
                col2Div.appendChild(inputCheckbox); // Ajouter la case à cocher à col-2

                // Ajouter les colonnes à la ligne
                lineDiv.appendChild(col1Div);
                lineDiv.appendChild(col2Div);

                // Ajouter la ligne à l'élément parent
                listeProcessusHtmlElement.appendChild(lineDiv);
            }
        }
    }
}