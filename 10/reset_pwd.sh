#!/bin/bash

# echo "������$1����"
smbpasswd -a $1

systemctl restart smb.service
