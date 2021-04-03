using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ThePatcher
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();

			MinimumSize = new Size(panel_wares.Width, MinimumSize.Height);

			list_wares.View = View.List;
			list_wares.ContextMenu = new ContextMenu(new MenuItem[] {
				new MenuItem("Refresh", RefreshScripts)
			});

			ImageList imgs = new ImageList();
			imgs.ImageSize = new Size(24, 24);

			list_wares.SmallImageList = imgs;
			list_wares.ItemSelectionChanged += List_wares_ItemSelectionChanged;

			ListAllScripts();
		}

		private void RefreshScripts(object sender, EventArgs e)
		{
			Program.FindScripts();
			ListAllScripts();
		}

		private void List_wares_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			if (list_wares.SelectedItems.Count < 1)
			{
				txt_desc.Text = txt_path.Text = lbl_name.Text = null;
				pnl_image.BackgroundImage = null;
				return;
			}

			ListViewItem item = list_wares.SelectedItems[0];
			PatcherScript script = Program.ScriptList.Find(x => x.Name == item.Text);

			if (script != null)
			{
				lbl_name.Text = script.Name;
				txt_path.Text = script.Script;
				txt_desc.Text = script.Desc.Replace("\\n", Environment.NewLine);

				if (script.Image != null)
					pnl_image.BackgroundImage = Image.FromFile(script.Image);
			}
		}

		public void ListAllScripts()
		{
			ImageList.ImageCollection imglist = list_wares.SmallImageList.Images;
			list_wares.Clear();
			imglist.Clear();

			int img_index = list_wares.SmallImageList.Images.Count;

			foreach (PatcherScript script in Program.ScriptList)
			{
				ListViewItem item = list_wares.Items.Add(script.Name);

				Image icon = Image.FromFile(script.Icon);
				if (icon != null)
				{
					imglist.Add(icon);
					item.ImageIndex = img_index++;
				}
			}
		}
	}
}
