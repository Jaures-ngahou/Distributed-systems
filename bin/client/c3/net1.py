from netconf_client.connect import connect_ssh
from netconf_client.ncclient import Manager

session = connect_ssh(host="192.168.43.187", port=830, username="jaures", password="jaures77")
mgr = Manager(session, timeout=120)

config_xml = """
<config>
    ...
</config>
"""
mgr.edit_config(config=config_xml)

filter_xml = """
<filter>
    ...
</filter>
"""
result = mgr.get(filter=filter_xml)
print(result.data_xml)
