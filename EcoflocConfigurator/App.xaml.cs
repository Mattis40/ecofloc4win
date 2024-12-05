using System;
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

            //Console.WriteLine("Répertoire de travail défini : " + Directory.GetCurrentDirectory());
        }
    }
}
