#!/bin/bash
sudo groupadd developpeurs
echo "le groupe developpeurs a été créé"
sudo chgrp developpeurs user.sh
echo "le fichier user.sh a été ajouté au groupe developpeurs"
sudo chmod 070 user.sh
echo "les droits d'accès du fichier user.sh ont été modifiés"
