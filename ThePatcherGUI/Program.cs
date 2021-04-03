using IniFile;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ThePatcher
{
	static class Program
	{
		public static List<PatcherScript> ScriptList = new List<PatcherScript>();

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			FindScripts();

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
		}

		public static void FindScripts()
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
		public static int LoadScriptsFile(string FullName)
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
	}
}
