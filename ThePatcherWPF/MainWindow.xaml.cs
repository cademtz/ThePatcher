using IniFile;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Drawing;
using ThePatcher;
using System.Diagnostics;

namespace ThePatcherWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private List<PatcherScript> ScriptList = new List<PatcherScript>();

		public MainWindow()
        {
            InitializeComponent();
			FindScripts();

			scriptList.ItemsSource = ScriptList;

			String local	= Directory.GetCurrentDirectory() + "/ThePatcherRuntime.exe";
			String compiled = Directory.GetCurrentDirectory() + "/../../../x64/Release/ThePatcherRuntime.exe";

			if (!File.Exists(local))
				if (File.Exists(compiled))
					File.Copy(compiled, local);

			if (!File.Exists(local))
            {
				MessageBox.Show("Failed to find patcher runtime.", "The Patcher");
				Environment.Exit(0);
            }				
		}

        public void FindScripts()
		{
			List<string[]> branches;
			string scripts = null;
			string[] subdirs = null;

			ScriptList.Clear();

			scripts = Array.Find(Directory.GetDirectories(Directory.GetCurrentDirectory()),
				x => x.EndsWith("\\ThePatcherScripts"));

			if (scripts == null || (subdirs = Directory.GetDirectories(scripts)).Length < 1)
				return;

			branches = new List<string[]>();
			branches.Add(subdirs);

			// Recurse directories
			for (int i = 0; i < branches.Count; i++)
			{
				foreach (string dir in branches.ElementAt(i))
				{
					subdirs = Directory.GetDirectories(dir);
					if (subdirs.Length > 0)
						branches.Add(subdirs);

					scripts = dir + "\\scripts.ini";
					if (File.Exists(scripts))
						LoadScriptsFile(scripts);
				}
			}
		}

		/// <summary> Loads one or more scripts' information from a 'scripts.ini' file </summary>
		/// <param name="FullName">Full path to a 'scripts.ini' file</param>
		/// <returns>
		/// Number of valid script infos loaded from a single file. <br></br>
		/// Zero means the file was not parsed correctly. <br></br>
		/// Under zero means the file could not be read. <br></br>
		/// </returns>
		public int LoadScriptsFile(string FullName)
		{
			string dir;
			string text;
			List<IniSection> ini;
			int count = 0;

			if (!File.Exists(FullName) || (text = File.ReadAllText(FullName)) == null)
				return -1;

			dir = FullName.Substring(0, FullName.LastIndexOf('\\'));
			text = text.Replace(System.Environment.NewLine, "\n");

			if ((ini = IniParse.FromString(text)) == null)
				return 0;

			foreach (IniSection sect in ini)
			{
				PatcherScript script = PatcherScript.FromIni(sect, dir);
				if (script != null)
				{
					ScriptList.Add(script);
					count++;
				}
			}

			return count;
		}

        private void executeBtn_Click(object sender, RoutedEventArgs e)
		{
			if (scriptList.SelectedIndex == -1)
				return;

			var scripts = (List<PatcherScript>)scriptList.ItemsSource;
			var script = scripts[scriptList.SelectedIndex];

			String local = Directory.GetCurrentDirectory() + "/ThePatcherRuntime.exe";
			Process.Start(local, "\"" + script.Script + "\"");
		}

		private void scriptList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
			var scripts = (List<PatcherScript>)scriptList.ItemsSource;
			var script = scripts[scriptList.SelectedIndex];

			executeBtn.Visibility = Visibility.Visible;

			scriptDesc.Text = script.Desc.Replace("\\n", Environment.NewLine);
			scriptImage.Source = script.Image == null ? null : new BitmapImage(new Uri(script.Image));
		}
    }
}
