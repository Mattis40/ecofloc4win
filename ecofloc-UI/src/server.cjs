const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // Pour permettre les requêtes depuis votre front-end

const app = express();
const port = 3000;

let processRunning = false; // Indique si le processus est en cours d'exécution
let configuratorRunning = false; // Indique si le configurator est en cours d'exécution

// Middleware
app.use(cors());
app.use(express.json());

// API pour lancer l'exécutable
app.post('/execute', (req, res) => {
    const exePath = `"${__dirname}\\testjson.exe"`; // Chemin absolu vers l'exécutable

    if (processRunning) {
        return res.status(400).json({ success: false, message: 'Un processus est déjà en cours.' });
    }

    try {
        // Lancer l'exécutable
        const process = exec(exePath, (error, stdout, stderr) => {
            processRunning = false; // Réinitialiser après exécution
            if (error) {
                console.error(`Erreur d'exécution : ${error.message}`);
                return; // Pas de `res.json` ici car la réponse a déjà été envoyée
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return; // Idem, gérer sans réponse supplémentaire
            }

            console.log(`Stdout : ${stdout}`);
        });

        processRunning = true; // Indique que le processus a démarré
        console.log('Processus lancé.');
        return res.json({ success: true, message: 'Processus lancé.' });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        processRunning = false; // Assurer que l'état est réinitialisé
        return res.status(500).json({ success: false, message: 'Erreur lors du lancement du processus.' });
    }
});

// API pour lancer le configurator
app.post('/configurator', (req, res) => {
    const exePath = `"${__dirname}\\EcoflocConfigurator.exe"`; // Chemin absolu vers le configurator

    if (configuratorRunning) {
        return res.status(400).json({ success: false, message: 'Configurator est déjà en cours.' });
    }

    try {
        const process = exec(exePath, (error, stdout, stderr) => {
            configuratorRunning = false; // Réinitialiser après exécution
            if (error) {
                console.error(`Erreur d'exécution : ${error.message}`);
                return;
            }
            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return;
            }
            console.log(`Stdout : ${stdout}`);
        });

        configuratorRunning = true;
        console.log('Configurator lancé.');
        return res.json({ success: true, message: 'Configurator lancé.' });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        configuratorRunning = false;
        return res.status(500).json({ success: false, message: 'Erreur lors du lancement du configurator.' });
    }
});


// API pour arrêter l'exécutable
app.post('/stop', (req, res) => {
    const exeName = 'testjson.exe'; // Nom du processus à tuer

    if (!processRunning) {
        return res.status(400).json({ success: false, message: 'Aucun processus en cours.' });
    }

    try {
        // Utilisation de taskkill pour arrêter le processus par nom
        exec(`taskkill /IM ${exeName} /F`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Erreur lors de l'arrêt : ${error.message}`);
                return res.status(500).json({ success: false, message: `Erreur : ${error.message}` });
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                // Optionnel : envoyer un avertissement
            }

            console.log(`Processus arrêté : ${stdout}`);
            processRunning = false; // Réinitialiser après l'arrêt
            return res.json({ success: true, message: 'Processus arrêté.' });
        });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        processRunning = false; // Assurer que l'état est réinitialisé
        return res.status(500).json({ success: false, message: 'Erreur lors de l\'arrêt du processus.' });
    }
});

// Démarrer le serveur
app.listen(port, () => {
    console.log(`Serveur en cours d'exécution sur http://localhost:${port}`);
});
