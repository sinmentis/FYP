"""
Package Class
include:

Index Number of Slave Board
Slave Board Type
Address
State
"""

class package:
    def __init__(self, num, board_type, address, state):
        self.num = num
        self.board_type = board_type
        self.address = address
        self.state = state
        
        
    def __str__(self):
        template = """
{slave_num:10}: {0:5}
{board_type:10}: {1:5}
{address:10}: {2:5}
{state:10}: {3:5}""".format(self.num, self.board_type, self.address, self.state)
        return template