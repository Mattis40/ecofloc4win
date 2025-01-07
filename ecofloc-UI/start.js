const { exec } = require('child_process');

// Run `npm run dev` to start the Vite development server
exec('npm run dev --prefix ./ecofloc-UI', (err, stdout, stderr) => {
  if (err) {
    console.error('Error:', err);
    return;
  }
  console.log('stdout:', stdout);
  console.error('stderr:', stderr);
});

exec('node run src/server.cjs', (err, stdout, stderr) => {
  if (err) {
    console.error('Error:', err);
    return;
  }
  console.log('stdout:', stdout);
  console.error('stderr:', stderr);
});

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
});