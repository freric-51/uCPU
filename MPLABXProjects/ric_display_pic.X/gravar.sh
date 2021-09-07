#!/usr/bin/env bash

cd "/home/ric/MPLABXProjects/ric_display_pic.X/dist/CONF_PIC16F627A/production/"
echo -e "pic linux ok?"
pk2cmd /?v
ls -la *.hex
pk2cmd -p
echo -e "encontrou PIC16F627A\nEscrever"
pk2cmd -PPIC16F627A -Fric_display_pic.X.production.hex -M

echo -e "encontrou PIC16F627A\nVerificar"
pk2cmd -PPIC16F627A -Fric_display_pic.X.production.hex -Y

