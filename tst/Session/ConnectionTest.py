from unittest import TestCase

from Config import *

from OpcUaTest.OpcUaService import OpcUaSessionService

class ConnectionTest(TestCase):
    
    
    def test_connect(self):
        
        # Connect to opc ua file server
        ip = {
            "ServerUrl": SERVER_URL
        }
        op = {}
        OpcUaSessionService.connect(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
    
    
    def test_disconnect(self):
        
        # Disconnect to opc ua file server
        ip = {}
        op = {}
        OpcUaSessionService.disconnect(ip, op)
        self.assertEqual(op['ResultCode'], 'OK')