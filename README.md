# Proyecto EspectoFotoColorímetro
## Introducción


## Descripción de pines
### Micro Master
<table>
    <tr>
        <th>Master</th>
        <th>Externo</th>
    </tr>
    <tr>
        <td>1.- PC6 (PCINT14/RESET) </td>
        <td>RESET</td>
    </tr>
    <tr>
        <td>2.- PD0 (PCINT16/RXD) </td>
        <td>TERMINAL - RXD.USART</td>
    </tr>
    <tr>
        <td>3.- PD1 (PCINT17/TXD) </td>
        <td>TERMINAL - TXD.USART</td>
    </tr>
    <tr>
        <td>4.- PD2 (PCINT18/INT0) </td>
        <td>LCD - R/W (5) </td>
    </tr>
    <tr>
        <td>5.- PD3 (PCINT19/INT1/OC2B) </td>
        <td>LCD - E (6) </td>
    </tr>
    <tr>
        <td>6.- PD4 (PCINT20/T0/XCK) </td>
        <td>LCD - D1 (11) </td>
    </tr>
    <tr>
        <td>7.- VCC </td>
        <td>VCC </td>
    </tr>
    <tr>
        <td>8.- GND </td>
        <td>GND </td>
    </tr>
    <tr>
        <td>9.- PB6 (PCINT6/XTAL1/TOSC1) </td>
        <td> </td>
    </tr>
    <tr>
        <td>10.- PB7 (PCINT7/XTAL2/TOSC2) </td>
        <td> </td>
    </tr>
    <tr>
        <td>11.- PD5 (PCINT21/T1/OC0B) </td>
        <td>LCD - D2 (12) </td>
    </tr>
    <tr>
        <td>12.- PD6 (PCINT22/AIN0/OC0A) </td>
        <td>LCD - D3 (13) </td>
    </tr>
    <tr>
        <td>13.- PD7 (PCINT23/AIN1) </td>
        <td>LCD - D4 (14) </td>
    </tr>
    <tr>
        <td>14.- PB0 (PCINT0/CLKO/ICP1) </td>
        <td> </td>
    </tr>
    <tr>
        <td>15.- PB1 (PCINT1/OC1A) </td>
        <td> </td>
    </tr>
    <tr>
        <td>16.- PB2 (PCINT2/OC1B/SS) </td>
        <td>SPI - SS (SLAVE.16) </td>
    </tr>
    <tr>
        <td>17.- PB3 (PCINT3/OC2A/MOSI) </td>
        <td>SPI - MOSI (SLAVE.17) </td>
    </tr>
    <tr>
        <td>18.- PB4 (PCINT4/MISO) </td>
        <td>SPI - MISO (SLAVE.18) </td>
    </tr>
    <tr>
        <td>19.- PB5 (PCINT5/SCK) </td>
        <td>SPI - SCK (SLAVE.19) </td>
    </tr>
    <tr>
        <td>20.- AVCC </td>
        <td> </td>
    </tr>
    <tr>
        <td>21.- AREF </td>
        <td> </td>
    </tr>
    <tr>
        <td>22.- GND </td>
        <td> </td>
    </tr>
    <tr>
        <td>23.- PC0 (PCINT8/ADC0) </td>
        <td>LAMP </td>
    </tr>
    <tr>
        <td>24.- PC1 (PCINT9/ADC2) </td>
        <td>LCD - RS (4) </td>
    </tr>
    <tr>
        <td>25.- PC2 (PCINT10/ADC2) </td>
        <td>PULSADOR - UP</td>
    </tr>
    <tr>
        <td>26.- PC3 (PCINT11/ADC3) </td>
        <td>PULSADOR - DOWN</td>
    </tr>
    <tr>
        <td>27.- PC4 (PCINT12/ADC4/SDA) </td>
        <td>PULSADOR - ENTER</td>
    </tr>
    <tr>
        <td>28.- PC5 (PCINT13/ADC5/SCL) </td>
        <td> </td>
    </tr>
</table>

### Micro Slave
<table>
    <tr>
        <th>Slave</th>
        <th>Externo</th>
    </tr>
    <tr>
        <td>1.- PC6 (PCINT14/RESET) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>2.- PD0 (PCINT16/RXD) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>3.- PD1 (PCINT17/TXD) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>4.- PD2 (PCINT18/INT0) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>5.- PD3 (PCINT19/INT1/OC2B) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>6.- PD4 (PCINT20/T0/XCK) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>7.- VCC </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>8.- GND </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>9.- PB6 (PCINT6/XTAL1/TOSC1) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>10.- PB7 (PCINT7/XTAL2/TOSC2) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>11.- PD5 (PCINT21/T1/OC0B) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>12.- PD6 (PCINT22/AIN0/OC0A) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>13.- PD7 (PCINT23/AIN1) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>14.- PB0 (PCINT0/CLKO/ICP1) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>15.- PB1 (PCINT1/OC1A) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>16.- PB2 (PCINT2/OC1B/SS) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>17.- PB3 (PCINT3/OC2A/MOSI) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>18.- PB4 (PCINT4/MISO) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>19.- PB5 (PCINT5/SCK) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>20.- AVCC </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>21.- AREF </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>22.- GND </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>23.- PC0 (PCINT8/ADC0) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>24.- PC1 (PCINT9/ADC2) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>25.- PC2 (PCINT10/ADC2) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>26.- PC3 (PCINT11/ADC3) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>27.- PC4 (PCINT12/ADC4/SDA) </td>
        <td>VCC</td>
    </tr>
    <tr>
        <td>28.- PC5 (PCINT13/ADC5/SCL) </td>
        <td>VCC</td>
    </tr>
</table>

</table>
