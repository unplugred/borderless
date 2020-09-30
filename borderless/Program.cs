using System;
using System.Windows.Forms;

namespace borderless
{
	static class Program
	{
		public static string pathhh;
		[STAThread]
		static void Main(string[] args)
		{
			if(args.Length >= 1) pathhh = args[0];
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
		}
	}
}
