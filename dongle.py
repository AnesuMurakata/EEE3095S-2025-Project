import PySimpleGUI as sg
import serial
import pyperclip
import time

# ---------- CONFIG ----------
COM_PORT = "COM3"
BAUD_RATE = 115200

# ---------- REAL DONGLE ----------
class Dongle:
    def __init__(self, port=COM_PORT, baud=BAUD_RATE):
        try:
            self.ser = serial.Serial(
                port=port,
                baudrate=baud,
                bytesize=8,
                parity='N',
                stopbits=1,
                timeout=1,
                xonxoff=False,
                rtscts=False,
                dsrdtr=False
            )
            self.connected = True
            # Clears startup messages
            time.sleep(0.5)
            self.ser.reset_input_buffer()
        except serial.SerialException:
            self.connected = False

    def send_command(self, cmd):
        """Send a text command to the STM dongle and read its response."""
        if not self.connected:
            return "ERR"
        try:
            self.ser.write((cmd + "\n").encode())
            time.sleep(0.1)
            resp = self.ser.readline().decode().strip()
            return resp if resp else "ERR"
        except Exception:
            return "ERR"

    def close(self):
        if self.connected:
            try:
                self.ser.write(b"DISCONNECT\n")
                self.ser.close()
            except Exception:
                pass

# ---------- GUI ----------
dongle = Dongle()

connect_layout = [
    [sg.Text("Dongle Lock System", font=("Helvetica", 16), justification="center")],
    [sg.Text("Click Connect to start", key="-STATUS-")],
    [sg.Button("Connect", size=(10, 1)), sg.Button("Exit", size=(10, 1))]
]

main_layout = [
    [sg.Text("Dongle Connected", font=("Helvetica", 16), justification="center")],
    [sg.Text("", key="-MESSAGE-", justification="center")],
    [sg.Button("Get code 1"), sg.Button("Get code 2"), sg.Button("Get code 3")],
    [sg.Button("Exit", button_color="red")]
]

window = sg.Window("Dongle Lock", connect_layout)
connected = False

# ---------- MAIN LOOP ----------
while True:
    event, values = window.read()
    if event in (sg.WIN_CLOSED, "Exit"):
        if connected:
            dongle.send_command("DISCONNECT")
            pyperclip.copy("")
            dongle.close()
        break

    if event == "Connect":

        resp = dongle.send_command("CONNECT")
        

        if resp != "ERR":
            connected = True
            window.close()
            window = sg.Window("Dongle Lock", main_layout, finalize=True)
            window["-MESSAGE-"].update("Ready - Click a code button")
        else:
            sg.popup_error("Device Not Found")

    elif event.startswith("Get code"):
        n = int(event[-1])
        resp = dongle.send_command(f"GET_CODE_{n}")

        if resp == "ERROR:NOT_SET":
            new_code = sg.popup_get_text(f"Slot {n} is empty. Enter new code:", title=f"Set Code {n}")
            
            if new_code:
                set_resp = dongle.send_command(f"SET_CODE_{n}:{new_code}")
                if set_resp == "OK":
                    pyperclip.copy(new_code)
                    window["-MESSAGE-"].update("Code Saved & Copied to Clipboard ✅")
                else:
                    window["-MESSAGE-"].update("Error saving code!")
            else:
                window["-MESSAGE-"].update("Code entry cancelled")
        elif resp.startswith("OK:"):
            code = resp.split(":", 1)[1]
            pyperclip.copy(code)
            window["-MESSAGE-"].update("Code in clipboard ✅")
        else:
            window["-MESSAGE-"].update(f"Unexpected: {resp}")

window.close()