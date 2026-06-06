using System;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Reflection;
using System.Windows.Forms;

class InstallerForm : Form {
    private Button btnInstall;
    private Label lblDesc;
    private PictureBox pbBanner;
    
    public InstallerForm() {
        this.Text = "PassVault Setup";
        this.Size = new Size(500, 350);
        this.FormBorderStyle = FormBorderStyle.FixedDialog;
        this.MaximizeBox = false;
        this.StartPosition = FormStartPosition.CenterScreen;
        this.BackColor = Color.White;
        
        try {
            // Using default icon if resources don't load
            this.Icon = new Icon(@"resources\icon.ico");
        } catch {}

        pbBanner = new PictureBox();
        pbBanner.Dock = DockStyle.Top;
        pbBanner.Height = 150;
        pbBanner.SizeMode = PictureBoxSizeMode.Zoom;
        try {
            pbBanner.Image = Image.FromFile(@"resources\banner.png");
        } catch {}
        this.Controls.Add(pbBanner);

        lblDesc = new Label();
        lblDesc.Text = "Welcome to PassVault!\n\nPassVault is a secure, local password manager designed to keep your credentials safe using strong AES-256 encryption. Your data is stored locally and never leaves your computer.\n\nClick 'Install' to extract PassVault to your Desktop.";
        lblDesc.Location = new Point(20, 170);
        lblDesc.Size = new Size(450, 80);
        lblDesc.Font = new Font("Segoe UI", 9.5f);
        this.Controls.Add(lblDesc);

        btnInstall = new Button();
        btnInstall.Text = "Install";
        btnInstall.Size = new Size(100, 35);
        btnInstall.Location = new Point(370, 260);
        btnInstall.Click += BtnInstall_Click;
        this.Controls.Add(btnInstall);
    }

    private void BtnInstall_Click(object sender, EventArgs e) {
        btnInstall.Enabled = false;
        btnInstall.Text = "Installing...";
        Application.DoEvents();

        string extractPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "PassVault");
        
        try {
            if (!Directory.Exists(extractPath)) {
                Directory.CreateDirectory(extractPath);
            }

            // Read the embedded zip from resources
            using (Stream stream = Assembly.GetExecutingAssembly().GetManifestResourceStream("passvault_installer.zip")) {
                if (stream == null) {
                    MessageBox.Show("Error: The embedded package 'passvault_installer.zip' was not found inside the executable.", "Extraction Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btnInstall.Enabled = true;
                    btnInstall.Text = "Install";
                    return;
                }
                
                using (ZipArchive archive = new ZipArchive(stream)) {
                    foreach (ZipArchiveEntry entry in archive.Entries) {
                        string fullPath = Path.Combine(extractPath, entry.FullName);
                        if (string.IsNullOrEmpty(entry.Name)) {
                            Directory.CreateDirectory(fullPath);
                        } else {
                            Directory.CreateDirectory(Path.GetDirectoryName(fullPath));
                            entry.ExtractToFile(fullPath, true);
                        }
                    }
                }
            }

            MessageBox.Show("PassVault has been successfully installed!\n\nYou can find the 'PassVault' folder on your Desktop.", "Installation Complete", MessageBoxButtons.OK, MessageBoxIcon.Information);
            this.Close();
        } catch (Exception ex) {
            MessageBox.Show("Installation failed:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            btnInstall.Enabled = true;
            btnInstall.Text = "Install";
        }
    }
}

class Program {
    [STAThread]
    static void Main() {
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        Application.Run(new InstallerForm());
    }
}
