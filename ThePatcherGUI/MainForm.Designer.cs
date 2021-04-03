
namespace ThePatcher
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.list_wares = new System.Windows.Forms.ListView();
			this.panel_wares = new System.Windows.Forms.Panel();
			this.lbl_name = new System.Windows.Forms.Label();
			this.pnl_image = new System.Windows.Forms.Panel();
			this.btn_runscript = new System.Windows.Forms.Button();
			this.txt_desc = new System.Windows.Forms.TextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.txt_path = new System.Windows.Forms.TextBox();
			this.panel_wares.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// list_wares
			// 
			this.list_wares.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.list_wares.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.list_wares.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.list_wares.Font = new System.Drawing.Font("Roboto Lt", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.list_wares.ForeColor = System.Drawing.Color.Gainsboro;
			this.list_wares.HideSelection = false;
			this.list_wares.Location = new System.Drawing.Point(12, 12);
			this.list_wares.Name = "list_wares";
			this.list_wares.Size = new System.Drawing.Size(217, 317);
			this.list_wares.TabIndex = 0;
			this.list_wares.UseCompatibleStateImageBehavior = false;
			// 
			// panel_wares
			// 
			this.panel_wares.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.panel_wares.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.panel_wares.Controls.Add(this.list_wares);
			this.panel_wares.Location = new System.Drawing.Point(0, 0);
			this.panel_wares.Name = "panel_wares";
			this.panel_wares.Size = new System.Drawing.Size(242, 341);
			this.panel_wares.TabIndex = 1;
			// 
			// lbl_name
			// 
			this.lbl_name.AutoSize = true;
			this.lbl_name.Font = new System.Drawing.Font("Roboto Lt", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lbl_name.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(225)))), ((int)(((byte)(225)))));
			this.lbl_name.Location = new System.Drawing.Point(3, 6);
			this.lbl_name.Name = "lbl_name";
			this.lbl_name.Size = new System.Drawing.Size(125, 25);
			this.lbl_name.TabIndex = 3;
			this.lbl_name.Text = "Script name";
			this.lbl_name.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// pnl_image
			// 
			this.pnl_image.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pnl_image.AutoScroll = true;
			this.pnl_image.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.pnl_image.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.pnl_image.Location = new System.Drawing.Point(253, 74);
			this.pnl_image.Name = "pnl_image";
			this.pnl_image.Size = new System.Drawing.Size(318, 138);
			this.pnl_image.TabIndex = 4;
			// 
			// btn_runscript
			// 
			this.btn_runscript.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.btn_runscript.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.btn_runscript.FlatAppearance.BorderSize = 0;
			this.btn_runscript.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btn_runscript.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(225)))), ((int)(((byte)(225)))));
			this.btn_runscript.Location = new System.Drawing.Point(253, 306);
			this.btn_runscript.Name = "btn_runscript";
			this.btn_runscript.Size = new System.Drawing.Size(318, 23);
			this.btn_runscript.TabIndex = 5;
			this.btn_runscript.Text = "Execute";
			this.btn_runscript.UseVisualStyleBackColor = false;
			// 
			// txt_desc
			// 
			this.txt_desc.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txt_desc.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.txt_desc.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txt_desc.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(225)))), ((int)(((byte)(225)))));
			this.txt_desc.Location = new System.Drawing.Point(253, 218);
			this.txt_desc.Multiline = true;
			this.txt_desc.Name = "txt_desc";
			this.txt_desc.ReadOnly = true;
			this.txt_desc.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.txt_desc.Size = new System.Drawing.Size(318, 82);
			this.txt_desc.TabIndex = 6;
			this.txt_desc.Text = "Script desc";
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.panel1.Controls.Add(this.txt_path);
			this.panel1.Controls.Add(this.lbl_name);
			this.panel1.Location = new System.Drawing.Point(253, 12);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(318, 56);
			this.panel1.TabIndex = 2;
			// 
			// txt_path
			// 
			this.txt_path.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txt_path.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
			this.txt_path.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txt_path.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(225)))), ((int)(((byte)(225)))));
			this.txt_path.Location = new System.Drawing.Point(8, 34);
			this.txt_path.Name = "txt_path";
			this.txt_path.ReadOnly = true;
			this.txt_path.Size = new System.Drawing.Size(307, 13);
			this.txt_path.TabIndex = 7;
			this.txt_path.Text = "Script path";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(54)))));
			this.ClientSize = new System.Drawing.Size(583, 341);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.txt_desc);
			this.Controls.Add(this.btn_runscript);
			this.Controls.Add(this.pnl_image);
			this.Controls.Add(this.panel_wares);
			this.ForeColor = System.Drawing.Color.Gainsboro;
			this.Name = "MainForm";
			this.ShowIcon = false;
			this.Text = "The patcher";
			this.panel_wares.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListView list_wares;
		private System.Windows.Forms.Panel panel_wares;
		private System.Windows.Forms.Label lbl_name;
		private System.Windows.Forms.Panel pnl_image;
		private System.Windows.Forms.Button btn_runscript;
		private System.Windows.Forms.TextBox txt_desc;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.TextBox txt_path;
	}
}

