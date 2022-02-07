import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

class Main {
    public static void main(String[] args) throws IOException {
        //System.out.println("Hello world!");

        String file = Files.readString(Path.of("test.lvl")); // Java 11 
        String[] lines = file.split("\n");
        //File.ReadAllLines("test.lvl");
        boolean isLVL = false;
        for (String line : lines)
        {
            //Console.WriteLine("\t" + line);

            if (line.equals("lvl1"))
            {
                isLVL = true;
                continue;
            }

            if (line.charAt(0) == '#')
            {
                continue;
            }

            if (isLVL)
            {
                // parsePos
                String model, name, tag; //0, 1, 2
                float x, y, z; //3,4,5
                float rx, ry, rz; //6,7,8
                float sx, sy, sz; //9,10,11
                String[] tmp = line.split(",");

                model = tmp[0];
                name = tmp[1];
                tag = tmp[2];
                x = Float.parseFloat(tmp[3]);
                y = Float.parseFloat(tmp[4]);
                z = Float.parseFloat(tmp[5]);
                rx = Float.parseFloat(tmp[6]);
                ry = Float.parseFloat(tmp[7]);
                rz = Float.parseFloat(tmp[8]);
                sx = Float.parseFloat(tmp[9]);
                sy = Float.parseFloat(tmp[10]);
                sz = Float.parseFloat(tmp[11]);

                System.out.println("OUTPUT:");
                System.out.println(" model: " + model + "\n name:" +name + "\n tag:" + tag);
                System.out.println(" pos:" + x + " " + y + " " + z);
                System.out.println(" rot:" + rx + " " + ry + " " + rz);
                System.out.println(" scale:" + sx + " " + sy + " " + sz);
            }
        }
    }
}