using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IniFile
{
	class IniSection
	{
		public string Name;
		public List<IniPair> Pairs;

		public IniSection(string Name) { this.Name = Name; Pairs = new List<IniPair>(); }

		public IniPair FindPair(string Key) => Pairs.Find(x => x.key == Key);
		public string FindValue(string Key)
		{
			IniPair pair = FindPair(Key);
			if (pair != null)
				return pair.val;
			return null;
		}
	}

	class IniPair {
		public IniPair(string Key, string Val) { key = Key; val = Val; }
		public string key, val;
	}

	static class IniParse
	{
		private static int NextLine(string Str, int Start)
		{
			if (Start >= Str.Length - 1 || Start < 0)
				return -1;
			else if (Str[Start] == '\n' || (Start = Str.IndexOf('\n', Start)) >= 0)
				return ++Start;
			return -1;
		}

		public static List<IniSection> FromString(string Str)
		{
			List<IniSection>	list = new List<IniSection>();
			IniSection			sect = null;
			int					pos = 0, next = 0;

			for (; pos >= 0 && pos < Str.Length; pos = NextLine(Str, pos))
			{
				switch(Str[pos])
				{
					case ';':
					case '\n':
						continue;

					case '[':
						pos++;
						if ((next = Str.IndexOf(']', pos)) >= 0)
						{
							sect = new IniSection(next > 0 ? Str.Substring(pos, next - pos) : "");
							list.Add(sect);
						}
						break;

					default:
						if ((next = Str.IndexOf('=', pos)) > pos)
						{
							string key = Str.Substring(pos, next - pos);

							pos = next + 1;
							if ((next = Str.IndexOf('\n', pos)) > pos ||
								(next = Str.Length) > pos + 1)
							{
								if (sect == null)
								{
									sect = new IniSection("");
									list.Add(sect);
								}

								sect.Pairs.Add(new IniPair(key, Str.Substring(pos, next - pos)));
							}
						}
						break;
				}
			}

			return list;
		}
	}
}
