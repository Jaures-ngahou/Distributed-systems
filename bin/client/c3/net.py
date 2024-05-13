from netconf_client.connect import connect_ssh
from netconf_client.ncclient import Manager

# Établir la connexion SSH avec le serveur Netconfd
session = connect_ssh(host="192.168.43.187", port=830, username="jaures", password="jaures77")
mgr = Manager(session, timeout=120)

# Définir la configuration XML pour la modification de l'interface docker0
config_xml = """
<config>
    <interfaces xmlns="urn:ietf:params:xml:ns:yang:ietf-interfaces">
        <interface>
            <name>docker0</name>
            <enabled>true</enabled>
            <ipv4>
                <address>
                    <ip>192.168.1.1</ip>
                    <prefix-length>24</prefix-length>
                </address>
            </ipv4>
        </interface>
    </interfaces>
</config>
"""

# Envoyer la configuration pour la modification de l'interface docker0
mgr.edit_config(config=config_xml)

# Afficher les informations sur l'interface docker0
filter_xml = """
<filter>
    <interfaces xmlns="urn:ietf:params:xml:ns:yang:ietf-interfaces">
        <interface>
            <name>docker0</name>
        </interface>
    </interfaces>
</filter>
"""
result = mgr.get(filter=filter_xml)
print(result.data_xml)

