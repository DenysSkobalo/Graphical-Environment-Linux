import subprocess
from tkinter import Tk, simpledialog, Button, Listbox, Label, Entry, messagebox

class WifiSelector:
    def __init__(self):
        self.root = Tk()
        self.root.title("Wi-Fi Networks")

        self.networks = self.get_wifi_networks()
        self.selected_network = None

        self.network_listbox = Listbox(self.root)
        self.network_listbox.pack()

        for network in self.networks:
            self.network_listbox.insert("end", network)

        self.connect_button = Button(self.root, text="Connect", command=self.connect_to_network)
        self.connect_button.pack()

        self.password_label = Label(self.root, text="Password:")
        self.password_label.pack()
        self.password_entry = Entry(self.root, show="*")
        self.password_entry.pack()

        self.root.mainloop()

    def get_wifi_networks(self):
        networks = subprocess.run(["nmcli", "-t", "-f", "SSID", "dev", "wifi", "list"], capture_output=True, text=True)
        networks_list = networks.stdout.strip().split('\n')
        return networks_list

    def connect_to_network(self):
        selected_index = self.network_listbox.curselection()
        if selected_index:
            self.selected_network = self.networks[selected_index[0]]
            password = self.password_entry.get()
            if password:
                subprocess.run(["nmcli", "device", "wifi", "connect", self.selected_network, "password", password], text=True)
                messagebox.showinfo("Connection", f"Connected to {self.selected_network}")
                self.root.destroy()
            else:
                messagebox.showerror("Error", "Please enter the password.")
        else:
            messagebox.showerror("Error", "Please select a Wi-Fi network.")

if __name__ == "__main__":
    wifi_selector = WifiSelector()

