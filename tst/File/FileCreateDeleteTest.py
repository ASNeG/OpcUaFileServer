from unittest import TestCase

from Config import *

from OpcUaTest.OpcUaService import OpcUaSessionService
from OpcUaTest.OpcUaFileSystem import OpcUaFileSystem


class FileCreateDeleteTest(TestCase):
    
    def setUp(self):
        
        # Connect to opc ua file server
        ip = {
            "ServerUrl": SERVER_URL
        }
        op = {}
        OpcUaSessionService.connect(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        
    def tearDown(self):
        
        # Disconnect to opc ua file server
        ip = {}
        op = {}
        OpcUaSessionService.disconnect(ip, op)
        self.assertEqual(op['ResultCode'], 'OK')
        
        
    def test_get_nodeid(self):
        
        # Open file system
        ip = {}
        op = {}
        OpcUaFileSystem.open(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        # Get nodeid from existing file
        ip = {
            "FileName": "CreateFile"
        }
        op = {}
        OpcUaFileSystem.getNodeId(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        
        # Get nodeid from not existing file
        ip = {
            "FileName": "NotExistingFile"
        }
        op = {}
        OpcUaFileSystem.getNodeId(ip, op);
        self.assertNotEqual(op['ResultCode'], 'OK')
        
        # Close file system 
        ip = {}
        op = {}
        OpcUaFileSystem.close(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
    
    
    def test_create_delete_file(self):
        
        # Open file system
        ip = {}
        op = {}
        OpcUaFileSystem.open(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        # Delete file without error handling
        ip = {
            "FileName": "TestFile"
        }
        op = {}
        OpcUaFileSystem.deleteFile(ip, op);
        
        # Create file
        ip = {
            "FileName": "TestFile"
        }
        op = {}
        OpcUaFileSystem.createFile(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        # Check if file exist
        ip = {
            "FileName": "TestFile"
        }
        op = {}
        OpcUaFileSystem.getNodeId(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        # delete file
        ip = {
            "FileName": "TestFile"
        }
        op = {}
        OpcUaFileSystem.deleteFile(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
        # Check if file exist
        ip = {
            "FileName": "TestFile"
        }
        op = {}
        OpcUaFileSystem.getNodeId(ip, op);
        self.assertNotEqual(op['ResultCode'], 'OK')
        
        # Close file system 
        ip = {}
        op = {}
        OpcUaFileSystem.close(ip, op);
        self.assertEqual(op['ResultCode'], 'OK')
        
