/* const { exec } = require('child_process');


// Run `npm run dev` to start the Vite development server
console.log("Running vite");
exec('npm run dev --prefix ./ecofloc-UI', (err, stdout, stderr) => {
  if (err) {
    console.error('Error:', err);
    return;
  }
  console.log('stdout:', stdout);
  console.error('stderr:', stderr);
});

// Run the Node.js server (server.cjs)
console.log("Starting Node server");
exec('node ./ecofloc-UI/src/server.cjs', (err, stdout, stderr) => {
  if (err) {
    console.error('Error starting Node server:', err);
    return;
  }
  console.log('Node server stdout:', stdout);
  console.error('Node server stderr:', stderr);
});

// Open Browser
console.log('Running on: http://localhost:5173/src/ \n');
exec(`start http://localhost:5173/src/`, (err, stdout, stderr) => {
  if (err) {
    console.error('Error opening browser:', err);
    return;
  }
  console.log('Browser opened:', stdout);
});

// Keep the terminal window open
const readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
}).question('Press ENTER to exit...', () => {
  process.exit();
}); */
const { exec } = require('child_process');
const readline = require('readline');

// Fonction pour exécuter une commande et afficher toutes les sorties
function runCommand(command, description) {
  console.log(`Starting: ${description}`);
  const process = exec(command, (err, stdout, stderr) => {
    if (err) {
      console.error(`Error in ${description}:`, err.message);
    }
    if (stderr) {
      console.error(`${description} stderr:`, stderr);
    }
    if (stdout) {
      console.log(`${description} stdout:`, stdout);
    }
  });
  process.stdout.on('data', (data) => {
    console.log(`[${description}] stdout: ${data.toString()}`);
  });

  process.stderr.on('data', (data) => {
    console.error(`[${description}] stderr: ${data.toString()}`);
  });

  process.on('error', (err) => {
    console.error(`Unexpected error in ${description}:`, err);
  });

  process.on('exit', (code, signal) => {
    console.log(`${description} exited with code ${code}, signal ${signal}`);
  });
}

// Démarrer Vite
runCommand('npm run dev --prefix ./ecofloc-UI', 'Vite Development Server');

// Démarrer le serveur Node.js
runCommand('node ./ecofloc-UI/src/server.cjs', 'Node.js Server');

// Ouvrir le navigateur
runCommand('start http://localhost:5173/src/', 'Browser');

// Garder la fenêtre de terminal ouverte
readline
  .createInterface({
    input: process.stdin,
    output: process.stdout,
  })
  .question('Press ENTER to exit...', () => {
    process.exit();
  });
