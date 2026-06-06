using System.IO;

class Program {
    static void Main() {
        byte[] pngData = File.ReadAllBytes(@"resources\icon.png");
        using (FileStream fs = new FileStream(@"resources\icon.ico", FileMode.Create))
        using (BinaryWriter bw = new BinaryWriter(fs)) {
            bw.Write((short)0); // reserved
            bw.Write((short)1); // image type (1 = icon)
            bw.Write((short)1); // number of images
            
            bw.Write((byte)0); // width (0 = 256)
            bw.Write((byte)0); // height (0 = 256)
            bw.Write((byte)0); // color count
            bw.Write((byte)0); // reserved
            bw.Write((short)1); // color planes
            bw.Write((short)32); // bits per pixel
            bw.Write((int)pngData.Length); // size of image data
            bw.Write((int)22); // offset of image data
            
            bw.Write(pngData);
        }
    }
}
