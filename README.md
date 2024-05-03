# soyosource-wifi-monitor
A project for read Inverter SoyoSource data via internet.

With this project you can read via internet the lcd of SoyoSource inverter.
You need to connect 4 wire in parallel to the original display cable WITHOUT disconnect the original display cable.

The sketch is designed to only read data so you need to connect only ground and RX to pin D2 of the ESP32.
This is the schema of display connector:


        │        │   R│ │
        │   CPU  │   S│ │
        └────────┘   4│ │
                     8│ │
            oooo     4│ │
            Disp        │
 ────────┐  ┌───┐  ┌────┘
         └──┘   └──┘ ││
          B+     B-

            ││││
            ││││            -------------
            ││││            |    ESP32   |
            │││└── RXD ---> | D2         |
            ││└─── TXD      |            |
            │└──── GND ---> |GND         |
            └───── 5V       |            |
                            --------------



It's better if you use a digital isolator module, as a ADUM1201. In that case you need to connect on the inverter side also the 5v from the display connector and the 3.3v from the esp32.
This is the better schema for connect the ESP32:


        │        │   R│ │
        │   CPU  │   S│ │
        └────────┘   4│ │
                     8│ │
            oooo     4│ │
            Disp        │
 ────────┐  ┌───┐  ┌────┘
         └──┘   └──┘ ││
          B+     B-

            ││││
            ││││            -------------
            ││││            | ADUM1201      |
            │││└── RXD ---> | VOA    VOA    |
            ││└─── TXD ---> | VIB    VIB    |
            │└──── GND ---> | GND1   GND2   |
            └───── 5V  ...> | VDD1   VDD2   |
                            --------------

But remember to don't disconnect the original display connector because this sketch don't send request data to the inverter, so if you disconnect the original connector you can't receive data.

After connect the ESP32 to a WIFI Network in Serial Monitor you can read the IP Address.
If you digit the address in a browser connected to the same wifi network you can read the data of the inverter.
Here some screenshot:

![pc-wide](https://github.com/Stefino76/soyosource-wifi-monitor/assets/52220779/07bf3f91-9429-4938-a330-044f132874f6)

![mobile](https://github.com/Stefino76/soyosource-wifi-monitor/assets/52220779/64552cd1-54fb-4bfe-af98-70a8bd810ab3)

If you have a static ip and open the port in your router you can read data from everywhere in the world.
