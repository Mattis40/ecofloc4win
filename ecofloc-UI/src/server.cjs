const express = require('express');
const { exec } = require('child_process');
const cors = require('cors'); // To enable queries from your front-end
const fs = require('fs');
const path = require('path');

const app = express();
const port = 3030;
const appProcessPath = './process.json';

let processRunning = false; // Indicates whether the process is running
let configuratorRunning = false; // Indicates whether the configurator is running

// Middleware
app.use(cors());
app.use(express.json());

// Changing dir by correct
const absolutePath = path.resolve('./ecofloc-UI/src');
if (fs.existsSync(absolutePath)) {
  process.chdir(absolutePath); // Change working directory
  console.log(`Work directory changed to : ${absolutePath}`);
} else {
  console.error(`The ${absolutePath} directory does not exist.`);
}

// API to launch the executable
app.post('/execute', (req, res) => {
    const exePath = `"${__dirname}\\Generator.exe"`; // Absolute path to executable

    if (processRunning) {
        return res.status(400).json({ success: false, message: 'Process already running' });
    }

    try {
        // Lancer l'ex�cutable
        const process = exec(exePath, (error, stdout, stderr) => {
            processRunning = false; 
            if (error) {
                console.error(`Execution error : ${error.message}`);
                return;
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return;
            }

            console.log(`Stdout : ${stdout}`);
        });

        processRunning = true;
        console.log('Process launched');
    } catch (error) {
        console.error(`Unexpected error : ${error.message}`);
        processRunning = false;
        return res.status(500).json({ success: false, message: 'Process startup error.' });
    }
});

// API pour lancer le configurator
app.post('/configurator', (req, res) => {
    const exePath = `"${__dirname}\\EcoflocConfigurator.exe"`; // Absolute path to configurator

    if (configuratorRunning) {
        return res.status(400).json({ success: false, message: 'Configurator already running' });
    }

    try {
        const process = exec(exePath, (error, stdout, stderr) => {
            configuratorRunning = false; 
            if (error) {
                console.error(`Execution error : ${error.message}`);
                return;
            }
            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return;
            }
            console.log(`Stdout : ${stdout}`);
        });

        configuratorRunning = true;
        console.log('Configurator started');
    } catch (error) {
        console.error(`Unexpected error : ${error.message}`);
        configuratorRunning = false;
        return res.status(500).json({ success: false, message: 'Error when launching configurator.' });
    }
});


// API pour arr�ter l'ex�cutable
app.post('/stop', (req, res) => {
    const exeName = 'Generator.exe'; // Name of process to kill

    if (!processRunning) {
        return res.status(400).json({ success: false, message: 'No process running' });
    }

    try {
        // Using taskkill to stop a process by name
        exec(`taskkill /IM ${exeName} /F`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Stop error : ${error.message}`);
                return res.status(500).json({ success: false, message: `Error : ${error.message}` });
            }

            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
            }

            console.log(`Process stopped : ${stdout}`);
            processRunning = false;
        });
    } catch (error) {
        console.error(`Unexpected error : ${error.message}`);
        processRunning = false;
        return res.status(500).json({ success: false, message: 'Error while attempting to stop the process.' });
    }
});

app.post('/changeListePidState', (req, res) => {
    const { liste, etat } = req.body;  // Retrieves sent data
    const data = JSON.parse(fs.readFileSync(appProcessPath, 'utf-8'));
    data.forEach(process => {
        if (liste.includes(process.name)) {
            process.pid.forEach(pidInfo => {
                pidInfo.checked = etat;
            });
        }
    });
    // Save changes to JSON file
    fs.writeFileSync(appProcessPath, JSON.stringify(data, null, 4), 'utf-8');
    
    res.json({ message: 'Liste reçue avec succès', receivedListe: liste });
});

app.post('/changePidState', (req, res) => {
    const { nomProc, pidProc, etat } = req.body; // Extract nomProc and pidProc from the request body

    const data = JSON.parse(fs.readFileSync(appProcessPath, 'utf-8'));
    data.forEach(process => {
        if (process.name == nomProc) {
            process.pid.forEach(pidInfo => {
                if (pidInfo.numeroPid == pidProc) {
                    pidInfo.checked = etat;
                }
            });
        }
    });
    // Save changes to JSON file
    fs.writeFileSync(appProcessPath, JSON.stringify(data, null, 4), 'utf-8');
    
    res.json({
        success: true,
        message: 'Process state changed successfully!',
        nomProc,
        pidProc,
        etat
    });
});

// SSE road
app.get('/events', (req, res) => {
    // Header configuration for SSE
    res.setHeader('Content-Type', 'text/event-stream');
    res.setHeader('Cache-Control', 'no-cache');
    res.setHeader('Connection', 'keep-alive');

    console.log('SSE customer connected');

    //  Sending an initial message
    res.write(`data: ${JSON.stringify({ message: 'Connection established' })}\n\n`);

    const filePath = './process.json';

    // Monitoring the JSON file
    const watcher = fs.watch(filePath, (eventType) => {
        if (eventType === 'change') {

            // Read file contents
            fs.readFile(filePath, 'utf8', (err, data) => {
                if (err) {
                    console.error('File read error :', err);
                    return;
                }

                // Check if the content is a valid JSON
                try {
                    const jsonData = JSON.parse(data); 
                    res.write(`data: ${JSON.stringify(jsonData)}\n\n`);
                } catch (parseErr) {
                    console.error('JSON parsing error :', parseErr);
                    res.write(`data: ${JSON.stringify({ error: 'Invalid JSON format' })}\n\n`);
                }
            });
        }
    });

    // Clean watcher when customer disconnects
    req.on('close', () => {
        console.log('SSE customer disconnected');
        watcher.close();
    });
});

// API pour lancer le configurator
app.post('/update', (req, res) => {
    const exePath = `"${__dirname}\\PIDRecup.exe"`; // Absolute path to configurator

     try {
        const process = exec(exePath, (error, stdout, stderr) => {
            configuratorRunning = false; // Reset after execution
            if (error) {
                console.error(`Execution error : ${error.message}`);
                return;
            }
            if (stderr) {
                console.warn(`Stderr : ${stderr}`);
                return;
            }
            console.log(`Stdout : ${stdout}`);
        });

        
    } catch (error) {
        console.error(`Unexpected error : ${error.message}`);
        configuratorRunning = false;
        return res.status(500).json({ success: false, message: 'Error when launching configurator.' });
    }
});


// D�marrer le serveur
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});
