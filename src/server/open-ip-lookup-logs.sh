##
# open-ip-lookup-logs.sh
#
# Jonathan Camenzuli
#
# Bash Script For Opening IP Lookup Server Logs
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 21/05/2023
#
#

clear

# Get ID of Container
ip_lookup_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model-ip-lookup-run')

# Open Container Logs
docker logs -f $ip_lookup_id