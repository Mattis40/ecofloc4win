function exporterCSV(){
    console.log("Recuperation des données");
    if (typeof graphCPU === 'undefined' || 
    typeof graphGPU === 'undefined' ||
    typeof graphNIC === 'undefined' ||
    typeof graphSD === 'undefined') {
        console.error('Data is not defined');
        return;
    }
    let donneesExporter = [["PID", "Temps", "Consomation CPU (W)", "Consomation GPU (W)","Consommation SD (W)", "Consommation NIC (W)"]];
    console.log(graphCPU);
    console.log(donneesExporter);
    Object.keys(graphCPU.data).forEach(function(PID) {
        console.log(PID);
        for (let i = 0; i < graphCPU.data[PID].y.length; i++) {
            let donneesPIDTemps = [PID, i, graphCPU.data[PID].y[i], graphGPU.data[PID].y[i], graphSD.data[PID].y[i], graphNIC.data[PID].y[i]];
            donneesExporter.push(donneesPIDTemps);
            //console.log(i); // Affiche les indices 0, 1, 2, 3, 4
        }
    });
    console.log(donneesExporter);
    const feuille1 = XLSX.utils.aoa_to_sheet(donneesExporter);
    const wb = XLSX.utils.book_new();
    XLSX.utils.book_append_sheet(wb, feuille1, "Data");
    XLSX.writeFile(wb, "export_data.xlsx");
    /*// Données pour la première feuille
    const donnees1 = [
        ["Nom", "Prénom", "Âge", "Ville"],
        ["Dupont", "Jean", 28, "Paris"],
        ["Martin", "Marie", 34, "Lyon"],
        ["Lemoine", "Paul", 22, "Marseille"]
    ];

    // Données pour la deuxième feuille
    const donnees2 = [
        ["PID", "Temps", "Consomation CPU (W)", "Consomation GPU (W)","Consommation SD (W)", "Consommation NIC (W)"],
        [9012, 1, 5, 10, 11, 2],
        [9020, 1, 6, 9, 4, 1],
        [1023, 1, 4, 6, 6, 1],
        [9012, 2, 7, 15, 9, 0],
        [9020, 2, 8, 3, 4, 0],
        [1023, 2, 6, 4, 5, 1]
    ];

    // Créer les feuilles
    const feuille1 = XLSX.utils.aoa_to_sheet(donnees1);
    const feuille2 = XLSX.utils.aoa_to_sheet(donnees2);

    // Créer un classeur avec les deux feuilles
    const wb = XLSX.utils.book_new();
    XLSX.utils.book_append_sheet(wb, feuille1, "Données Personnelles");
    XLSX.utils.book_append_sheet(wb, feuille2, "Produits");

    // Exporter le classeur en tant que fichier Excel
    XLSX.writeFile(wb, "exemple_multiple_feuilles.xlsx");*/
}