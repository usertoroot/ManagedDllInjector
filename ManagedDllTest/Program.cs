using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ManagedDllTest
{
    class Program
    {
        static void DllMain(string arg)
        {
            MessageBox.Show("YAY! Loaded! " + arg);
        }

        static void Main(string[] args)
        {
            MessageBox.Show("YAY! Loaded!");
        }
    }
}
