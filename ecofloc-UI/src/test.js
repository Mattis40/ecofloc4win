async function monitoring(){

    const command = 'echo hello'; // Replace with your desired shell command

    try {
      const response = await fetch('http://localhost:3000/execute', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ command }),
      });

      const data = await response.json();

      if (response.ok) {
        console.log(`Output:\n${data.stdout}`);
      } else {
        console.log(`Error:\n${data.error || data.stderr}`);
      }
    } catch (error) {
      console.log(`Request failed: ${error.message}`);
    }
  };


