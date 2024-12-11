const { exec } = require('child_process');
//const open = require('open');

// Run `npm run dev` to start the Vite development server
exec('npm run dev', (err, stdout, stderr) => {
  if (err) {
    console.error('Error:', err);
    return;
  }
  console.log('stdout:', stdout);
  console.error('stderr:', stderr);

  // Open index.html (or your default URL) in the browser
  //open('http://localhost:5173/src/'); 
});
/*exec('node run src/exec-ecofloc.cjs', (err, stdout, stderr) => {
  if (err) {
    console.error('Error:', err);
    return;
  }
  console.log('stdout:', stdout);
  console.error('stderr:', stderr);

  // Open index.html (or your default URL) in the browser
  open('http://localhost:5173/src/'); 
});*/
console.log('Running on: http://localhost:5173/src/ \n');
// Keep the terminal window open
const readline = require('readline');
readline.createInterface({
  input: process.stdin,
  output: process.stdout
}).question('Press ENTER to exit...', () => {
  process.exit();
});