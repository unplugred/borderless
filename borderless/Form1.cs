using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Windows.Forms;

namespace borderless
{
	public partial class Form1 : Form
	{
		float scale = 1;
		string prevfile;
		string currentfile;
		string nextfile;
		MenuItem interpolate;
		MenuItem alwaysontop;
		bool isontop = false;
		public static bool isinterpolate = true;

		public Form1()
		{
			InitializeComponent();

			SetStyle(ControlStyles.UserPaint, true);
			SetStyle(ControlStyles.SupportsTransparentBackColor, true);
			BackColor = Color.Transparent;

			if (Program.pathhh != null)
				openimage(Program.pathhh);

			ContextMenu cm = new ContextMenu();
			cm.MenuItems.Add("Open", new EventHandler(openpic));
			cm.MenuItems.Add("Next", new EventHandler(opennext));
			cm.MenuItems.Add("Previous", new EventHandler(openprev));
			cm.MenuItems.Add("-");
			cm.MenuItems.Add("Copy (transcoded)", new EventHandler(copyimg));
			cm.MenuItems.Add("Show in folder", new EventHandler(openfolder));
			cm.MenuItems.Add("-");
			interpolate = cm.MenuItems.Add("Nearest neighbor", new EventHandler(changeinterpolate));
			alwaysontop = cm.MenuItems.Add("Always on top", new EventHandler(changeontop));
			cm.MenuItems.Add("-");
			cm.MenuItems.Add("Quit", new EventHandler(closeapp));
			pictureBox1.ContextMenu = cm;

			pictureBox1.MouseWheel += pictureBox1_MouseWheel;
		}

		protected override void OnPaintBackground(PaintEventArgs e) { /* Ignore */ }

		void copyimg(object sender, EventArgs e)
		{
			Clipboard.Clear();
			Clipboard.SetImage(pictureBox1.Image);
		}

		void openfolder(object sender, EventArgs e)
		{
			if(currentfile != null) Process.Start("explorer.exe", "/select," + currentfile);
		}

		void changeontop(object sender, EventArgs e)
		{
			isontop = !isontop;
			if (isontop)
			{
				TopMost = true;
				alwaysontop.Text = "Sometimes on top";
			} else
			{
				TopMost = false;
				alwaysontop.Text = "Always on top";
			}
		}

		void changeinterpolate(object sender, EventArgs e)
		{
			isinterpolate = !isinterpolate;
			interpolate.Text = isinterpolate ? "Nearest neighbor" : "Bilinear";
			pictureBox1.Invalidate();
		}

		private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
		{
			openimage(openFileDialog1.FileName);
		}

		void openimage(string pathh)
		{
			try
			{
				string[] filelist = Directory.GetFiles(Path.GetDirectoryName(pathh));
				bool found = false;
				bool seekinglast = false;
				nextfile = null;
				prevfile = null;
				currentfile = pathh;
				foreach(string fil in filelist)
				{
					if (Path.GetFileName(fil) == Path.GetFileName(pathh))
					{
						found = true;
						currentfile = fil;
					} else if (compatible(fil)) {
						if (found)
						{
							if (seekinglast) prevfile = fil;
							else
							{
								nextfile = fil;
								if (prevfile != null) break;
								else
								{
									prevfile = fil;
									seekinglast = true;
								}
							}
						}
						else
						{
							prevfile = fil;
							if (nextfile == null) nextfile = fil;
						}
					}
				}
				Text = Path.GetFileName(pathh);

				if (pictureBox1.Image != null) pictureBox1.Image.Dispose();
				pictureBox1.Image = Image.FromFile(pathh);

				scale = 1;
				Rectangle currentmonitor = Screen.FromControl(this).WorkingArea;
				if (pictureBox1.Image.Width * scale > currentmonitor.Width)
					scale = (float)currentmonitor.Width / pictureBox1.Image.Width;
				if (pictureBox1.Image.Height * scale > currentmonitor.Height)
					scale = (float)currentmonitor.Height / pictureBox1.Image.Height;
				scaleimg();
				if (currentmonitor.Left > Left)
					Left = currentmonitor.Left;
				else if (currentmonitor.Right < Right)
					Left = currentmonitor.Right - Width;
				if (currentmonitor.Top > Top)
					Top = currentmonitor.Top;
				else if (currentmonitor.Bottom < Bottom)
					Top = currentmonitor.Bottom - Height;
			} catch(Exception e) {
				pictureBox1.Image = null;
			}
		}

		bool compatible(string pathh)
		{
			return pathh.EndsWith(".png") || pathh.EndsWith(".gif") || pathh.EndsWith(".jpg") || pathh.EndsWith(".jpeg") || pathh.EndsWith(".bmp") || pathh.EndsWith(".wmf");
		}

		void openpic(object sender, EventArgs e)
		{
			openFileDialog1.ShowDialog();
		}

		void opennext(object sender = null, EventArgs e = null)
		{
			if (nextfile != null) openimage(nextfile);
		}

		void openprev(object sender = null, EventArgs e = null)
		{
			if (prevfile != null) openimage(prevfile);
		}

		private bool mouseDown;
		private Point lastLocation;

		private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
		{
			mouseDown = true;
			lastLocation = e.Location;
		}

		private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
		{
			if (mouseDown)
			{
				Location = new Point(
					(Location.X - lastLocation.X) + e.X, (Location.Y - lastLocation.Y) + e.Y);

				Update();
			}
		}

		private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			mouseDown = false;
		}

		private void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
		{
			if (pictureBox1.Image == null) return;
			scale += (e.Delta * .2f)/pictureBox1.Image.Width;
			scaleimg((float)e.X / Width,(float)e.Y/Height);
		}

		void scaleimg(float anchorx = .5f, float anchory = .5f) {
			if (pictureBox1.Image.Width * scale < 32)
				scale = 32f / pictureBox1.Image.Width;
			if (pictureBox1.Image.Height * scale < 32)
				scale = 32f / pictureBox1.Image.Height;

			int prevwidth = Width;
			int prevheight = Height;
			int newwidth = (int)Math.Round(pictureBox1.Image.Width * scale);
			int newheight = (int)Math.Round(pictureBox1.Image.Height * scale);
			Width = newwidth;
			Height = newheight;
			Location = new Point(Location.X + (int)((prevwidth - newwidth)*anchorx), Location.Y + (int)((prevheight - newheight)*anchory));
		}

		void closeapp(object sender = null, EventArgs e = null)
		{
			Application.Exit();
		}

		private void keypress(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Right) opennext();
			if (e.KeyCode == Keys.Left) openprev();
			if (e.KeyCode == Keys.Escape) closeapp();
		}

		private void Form1_DragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.Copy;
		}

		private void Form1_DragDrop(object sender, DragEventArgs e)
		{
			openimage(((string[])e.Data.GetData(DataFormats.FileDrop))[0]);
		}
	}
}

public class PictureBoxWithInterpolationMode : PictureBox
{
	public PictureBoxWithInterpolationMode()
	{
		SetStyle(ControlStyles.UserPaint, true);
		SetStyle(ControlStyles.SupportsTransparentBackColor, true);
		BackColor = Color.Transparent;
	}
	protected override void OnPaintBackground(PaintEventArgs e) { /* Ignore */ }
	protected override void OnPaint(PaintEventArgs paintEventArgs)
	{
		paintEventArgs.Graphics.InterpolationMode = borderless.Form1.isinterpolate ? InterpolationMode.Bilinear : InterpolationMode.NearestNeighbor;
		paintEventArgs.Graphics.Clear(Color.Transparent);
		base.OnPaint(paintEventArgs);
	}
}