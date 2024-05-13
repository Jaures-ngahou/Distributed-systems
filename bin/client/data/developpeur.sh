#!/bin/bash
echo -n  "entrer un nom d'utilisateur pour le compte à créer :"
read nom
sudo useradd -m -d /home/$nom -s /bin/bash $nom -c "Linux User $nom"
echo "L'utilisateur $nom a été créé avec succes"
sudo usermod -aG developpeurs  $nom
echo "L'utilisateur $nom a été ajouté au groupe developpeurs"
