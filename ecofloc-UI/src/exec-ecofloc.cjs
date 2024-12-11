const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // Pour les requêtes cross-origin depuis un front-end

const app = express();
const port = 3000;

// Middleware
app.use(cors());
app.use(express.json());

// API Endpoint pour exécuter un fichier .exe
app.post('/execute', (req, res) => {
    const exePath = 'echo hello'; // Remplacez par le chemin absolu de votre fichier .exe

    exec(`"${exePath}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Erreur d'exécution: ${error.message}`);
            return res.status(500).send({ error: error.message });
        }

        if (stderr) {
            console.warn(`Stderr: ${stderr}`);
            return res.status(400).send({ stderr });
        }

        console.log(`Stdout: ${stdout}`);
        res.send({ stdout });
    });
});

// Démarrer le serveur
app.listen(port, () => {
    console.log(`Serveur en cours d'exécution sur http://localhost:${port}`);
});
