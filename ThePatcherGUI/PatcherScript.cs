using IniFile;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ThePatcher
{
	class PatcherScript
	{
		public string Script	{ get; private set; }
		public string Name		{ get; private set; }
		public string Desc		{ get; private set; }
		public string Icon		{ get; private set; }
		public string Image		{ get; private set; }

		public static PatcherScript FromIni(IniSection Ini, string IniDir = null)
		{
			string name = Ini.Name;
			string script = Ini.FindValue("script");
			string desc = Ini.FindValue("desc");
			string icon = Ini.FindValue("icon");
			string img = Ini.FindValue("image");
			PatcherScript result;

			script = TryFile(script, IniDir);

			if (script == null)
				return null;

			icon = TryFile(icon, IniDir);
			img = TryFile(img, IniDir);

			if (name == null || name.Length < 1)
				name = script;
			if (desc.Length < 1)
				desc = null; // Indicate "No description provided" with null

			result = new PatcherScript {
				Script = script,
				Name = name,
				Desc = desc,
				Icon = icon,
				Image = img
			};

			return result;
		}

		private PatcherScript() { }
		static private string TryFile(string Path, string IniDir)
		{
			if (Path != null && !File.Exists(Path))
			{
				if (IniDir == null || !File.Exists(Path = IniDir + '\\' + Path))
					return null;
			}
			return Path;
		}
	}
}
