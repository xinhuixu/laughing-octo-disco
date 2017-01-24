# Systems final project - Nala Sharadjaya, Xinhui Xu

### How-to
1. Compile and run `server` and `client` from two different terminal prompts.
(1b. Type `./client [manual ip]` for functionality on ANY computer!)
2. In `client`, create a new user by typing in whatever username you want!
3. Create a new project by following the prompts (create multiple if you want!). Then view your projects following the prompts.
4. You can view your projects, enter them, and view your tasks. Other features vary in functionality...if you don't see a response you expect, you can type `home` to see if that will redirect you. Otherwise `CTRL+C` and start over!

### Things that will work on a scale of probably to definitely
* Creating a new user (def)
* Creating a new proj (def)
* Viewing all the tasks (prob?)
* Viewing all YOUR tasks (def)

### Things we didn't have time to properly integrate
* Adding members and tasks (control flow turned out to be WICKED)
* Editing status of tasks (^^)
* Removing things (tasks, projects, users)

### Things we did :)
* Set up a clever and interesting control flow that turned out to be a lot more challenging than we expected!! Using numbers to tell the server where to go next requires forcing the server to pay SUPER close attention to where it is right now, and when the control flow isn't linear, that's mad hard.
* Permissions when accessing projects and tasks. This was tedious and took longer than we predicted - figuring out who can access what where and when, setting up the directory structure (under `users/`)...it took a while!
* Literally string manipulation in C. Was so hard!!!!