const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // Pour permettre les requ�tes depuis votre front-end

const app = express();
const port = 3030;

let processRunning = false; // Indique si le processus est en cours d'ex�cution
let configuratorRunning = false; // Indique si le configurator est en cours d'ex�cution

// Middleware
app.use(cors());
app.use(express.json());

// API pour lancer l'ex�cutable
app.post('/execute', (req, res) => {
    const exePath = `"${__dirname}\\testjson.exe"`; // Chemin absolu vers l'ex�cutable

    if (processRunning) {
        return res.status(400).json({ success: false, message: 'Un processus est d�j� en cours.' });
    }

    try {
        // Lancer l'ex�cutable
        const process = exec(exePath, (error, stdout, stderr) => {
            processRunning = false; // R�initialiser apr�s ex�cution
            if (error) {
                console.error(`Erreur d'ex�cution : ${error.message}`);
                return; // Pas de `res.json` ici car la r�ponse a d�j� �t� envoy�e
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return; // Idem, g�rer sans r�ponse suppl�mentaire
            }

            console.log(`Stdout : ${stdout}`);
        });

        processRunning = true; // Indique que le processus a d�marr�
        console.log('Processus lanc�.');
        return res.json({ success: true, message: 'Processus lanc�.' });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        processRunning = false; // Assurer que l'�tat est r�initialis�
        return res.status(500).json({ success: false, message: 'Erreur lors du lancement du processus.' });
    }
});

// API pour lancer le configurator
app.post('/configurator', (req, res) => {
    const exePath = `"${__dirname}\\EcoflocConfigurator.exe"`; // Chemin absolu vers le configurator

    if (configuratorRunning) {
        return res.status(400).json({ success: false, message: 'Configurator est d�j� en cours.' });
    }

    try {
        const process = exec(exePath, (error, stdout, stderr) => {
            configuratorRunning = false; // R�initialiser apr�s ex�cution
            if (error) {
                console.error(`Erreur d'ex�cution : ${error.message}`);
                return;
            }
            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return;
            }
            console.log(`Stdout : ${stdout}`);
        });

        configuratorRunning = true;
        console.log('Configurator lanc�.');
        return res.json({ success: true, message: 'Configurator lanc�.' });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        configuratorRunning = false;
        return res.status(500).json({ success: false, message: 'Erreur lors du lancement du configurator.' });
    }
});


// API pour arr�ter l'ex�cutable
app.post('/stop', (req, res) => {
    const exeName = 'testjson.exe'; // Nom du processus � tuer

    if (!processRunning) {
        return res.status(400).json({ success: false, message: 'Aucun processus en cours.' });
    }

    try {
        // Utilisation de taskkill pour arr�ter le processus par nom
        exec(`taskkill /IM ${exeName} /F`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Erreur lors de l'arr�t : ${error.message}`);
                return res.status(500).json({ success: false, message: `Erreur : ${error.message}` });
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                // Optionnel : envoyer un avertissement
            }

            console.log(`Processus arr�t� : ${stdout}`);
            processRunning = false; // R�initialiser apr�s l'arr�t
            return res.json({ success: true, message: 'Processus arr�t�.' });
        });
    } catch (error) {
        console.error(`Erreur inattendue : ${error.message}`);
        processRunning = false; // Assurer que l'�tat est r�initialis�
        return res.status(500).json({ success: false, message: 'Erreur lors de l\'arr�t du processus.' });
    }
});

// D�marrer le serveur
app.listen(port, () => {
    console.log(`Serveur en cours d'ex�cution sur http://localhost:${port}`);
});
