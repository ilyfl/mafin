# mafin

Fast and simple finances  

Expense types:  
- Food
- Eating out
- Transport
- Bills
- Clothes
- Health
- Phone
- Toiletry
- Other
May be more  

Income types:  
- Salary
- Wages  
- Random  
may be more  
I

TODO:  

- Prompt user for information: in/outcome? Category? Payload? Comment(optional)? Resource?  
- Graphs, graphical representation of the information  
- Show user free money  
- Show user financial prediction (how much he can spend money, like (known salary + average random incomes))  
- Currency convertation (sort of API for handling currency)
- Calculate the best way to gain some amount of money

LINUX (CLI):  
- Save info into sql  
- Client-server model (user connects to the remote database, server stores information given, processes info)  


MOBILE (GUI):  
- Simple gui  
- Local mode (save info into local file, user at first startup creates its own file)  
- Client-server mode (user connects to the remote database, server stores information given, processes info)  
- Client-server + local (when its possible connect to the server, if no respond save into temporary recent changes file)  

WINDOWS(GUI):
- dunno what is windows



Version 0.1:  
- ONLY CLI(linux)  
- Config file for stuff like path to the database
- Local db as a file (not sql)   
- User has to specify file directory location and name for the first time(only once)  
- No password  
- Program asks if user wanted to see stored information or input new one  
- Program prompts user for that information: income/outcome? category? payload? comment(optional)? resource?   
- Program saves the information into the csv file, all information comma-separated and format like this:   
date(DD/MM/YYY): (0-1)income/outcome(uint8_t), (0-255)category(uint8_t), payload(float 4 bytes), comment(string), (0-255)resource(uint8_t)\n  
- We need a little parser to get stored information into the memory and display it to the user  
- A function that interpret parsed information to display it in Human readable format  
- Output must contain information about how much user gained/spent money, last 10 operations (it's better to separate incomes/ outcomes)    
- User should be able to see his history(like with pager less)  
