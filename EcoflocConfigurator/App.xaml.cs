using System.Diagnostics;
using System.IO;
using System.Windows;

namespace EcoflocConfigurator
{
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            // Afficher le répertoire de travail pour le débogage
            //Console.WriteLine("Répertoire de travail initial : " + Directory.GetCurrentDirectory());

            // Changer explicitement le répertoire de travail
            string currentDirectory = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            Directory.SetCurrentDirectory(currentDirectory);

            // Console.WriteLine("Répertoire de travail défini : " + Directory.GetCurrentDirectory());

            // Verifier si une instance de l'application est déjà en cours d'exécution
            var existingProcess = Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName)
                .Where(p => p.Id != Process.GetCurrentProcess().Id)
                .FirstOrDefault();

            if (existingProcess == null)
            {
                // Si aucune instance n'existe
            }
            else
            {
                // Si une instance existe
                IntPtr hwnd = existingProcess.MainWindowHandle;
                if (hwnd != IntPtr.Zero)
                {
                    // Remettre la fenêtre existante au premier plan
                    ShowWindow(hwnd, 5); // 5 = SW_RESTORE = Restore the window to its normal state
                    SetForegroundWindow(hwnd);
                }
                Current.Shutdown(); // Fermer l'application actuelle
            }
        }

        // Imports pour gérer les fenêtres de l'application
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hwnd, int nCmdShow);
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        static extern bool SetForegroundWindow(IntPtr hwnd);
    }
}
