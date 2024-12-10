const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // Pour permettre les requêtes depuis votre front-end

const app = express();
const port = 3000;

// Middleware
app.use(cors());
app.use(express.json());

// API Endpoint pour exécuter le fichier testjson.exe dans le même dossier
app.post('/execute', (req, res) => {
    const exePath = './testjson.exe'; // Utilisation du chemin relatif pour le fichier .exe dans le même dossier

    exec(`"${exePath}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Erreur d'exécution: ${error.message}`);
            return res.status(500).json({ success: false, message: error.message });
        }

        if (stderr) {
            console.warn(`Stderr: ${stderr}`);
            return res.status(400).json({ success: false, message: stderr });
        }

        console.log(`Stdout: ${stdout}`);
        res.json({ success: true, stdout });
    });
});

// API Endpoint pour arrêter le processus
app.post('/stop', (req, res) => {
    if (!currentProcess) {
        return res.status(400).json({ success: false, message: 'Aucun processus en cours à arrêter.' });
    }

    // Tuer le processus en cours
    currentProcess.kill();

    res.json({ success: true, message: 'Processus arrêté avec succès.' });
});

// Démarrer le serveur
app.listen(port, () => {
    console.log(`Serveur en cours d'exécution sur http://localhost:${port}`);
});
