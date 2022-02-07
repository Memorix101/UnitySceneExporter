using System;
using System.IO;

class Program
{
    public static void Main(string[] args)
    {
        Console.WriteLine("Hello World");
        string[] lines = File.ReadAllLines("test.lvl");

        bool isLVL = false;
        foreach (string line in lines)
        {
            //Console.WriteLine("\t" + line);

            if (line == "lvl1")
            {
                isLVL = true;
                continue;
            }

            if (line[0] == '#')
            {
                continue;
            }

            if (isLVL)
            {
                // parsePos
                string model, name, tag; //0, 1, 2
                float x, y, z; //3,4,5
                float rx, ry, rz; //6,7,8
                float sx, sy, sz; //9,10,11
                string[] tmp = line.Split(',');

                model = tmp[0];
                name = tmp[1];
                tag = tmp[2];
                x = float.Parse(tmp[3], System.Globalization.CultureInfo.InvariantCulture);
                y = float.Parse(tmp[4], System.Globalization.CultureInfo.InvariantCulture);
                z = float.Parse(tmp[5], System.Globalization.CultureInfo.InvariantCulture);
                rx = float.Parse(tmp[6], System.Globalization.CultureInfo.InvariantCulture);
                ry = float.Parse(tmp[7], System.Globalization.CultureInfo.InvariantCulture);
                rz = float.Parse(tmp[8], System.Globalization.CultureInfo.InvariantCulture);
                sx = float.Parse(tmp[9], System.Globalization.CultureInfo.InvariantCulture);
                sy = float.Parse(tmp[10], System.Globalization.CultureInfo.InvariantCulture);
                sz = float.Parse(tmp[11], System.Globalization.CultureInfo.InvariantCulture);

                System.Console.WriteLine("OUTPUT:");
                System.Console.WriteLine($" model: {model}\n name:  {name}\n tag: {tag}");
                System.Console.WriteLine($" pos:  {x} {y} {z}");
                System.Console.WriteLine($" rot: {rx} {ry} {rz}");
                System.Console.WriteLine($" scale: {sx} {sy} {sz} ");
            }
        }
    }
}