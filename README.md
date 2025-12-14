
# IP Messenger (C++)

A simple IP-based messenger in C++ that allows users to:

- Maintain a contact list (name + IP + port)
- Add, remove, edit, and list contacts
- Send messages in XMPP-style XML format
- Receive messages in real-time on a separate thread

---

## Features

### Contact Management
- **Add contact:** store name, IP, and port  
- **Edit contact:** modify name, IP, or port  
- **Remove contact**  
- **List contacts**

### Messaging
- Send messages to contacts  
- Messages are formatted as:

```xml
<message to="IP" from="Name" type="chat" xmlns="jabber:client">
    <body>Your message here</body>
</message>
```

### Multi-terminal / Multi-user Support
- Each instance listens on a user-defined port  
- Messages are delivered to the correct IP + port of contacts

---

## Setup

### Compile the program

```bash
make
```

This generates the executable `ip_messenger`.

### Run multiple instances (for testing)

```bash
./ip_messenger
```

1. Enter your listening port (e.g., `5000`)  
2. Enter your name (e.g., `Alice`)  

Repeat in another terminal for another user (e.g., `Bob`) with a different port (e.g., `5001`).

---

## Menu Commands

After starting the program, the main menu is:

```text
--- IP Messenger ---
1. Add contact
2. List contacts
3. Remove contact
4. Edit contact
5. Send message
6. Exit
>
```

### 1. Add Contact
Adds a new contact with name, IP, and port.

Example:

```text
Name: Bob
IP: 127.0.0.1
Port: 5001
```

Contact is saved to `contacts.txt` and can be used to send messages.

**Expected Output:**

```text
> 1
Name: Bob
IP: 127.0.0.1
Port: 5001
Contact added successfully.
```

### 2. List Contacts
Shows all saved contacts with their IP and port.

**Example Output:**

```text
--- Contacts ---
1. Bob - 127.0.0.1:5001
```

### 3. Remove Contact
Deletes a contact by its number in the list.

**Example Flow:**

```text
Select contact number: 1
Contact removed successfully.
```

### 4. Edit Contact
Modify a contact’s name, IP, or port.

**Example Flow:**

```text
Select contact number: 1
New Name: Bob_the_friend
New IP: 127.0.0.1
New Port: 5001
Contact updated successfully.
```

### 5. Send Message
Sends a message to a contact using TCP. Messages are formatted in XMPP-style XML.

**Example Flow (Alice → Bob):**

```text
--- Contacts ---
1. Bob - 127.0.0.1:5001
Select contact number: 1
Message: Hi Bob!
```

**Bob Terminal Output:**

```text
[New Message]: <message to="127.0.0.1" from="Alice" type="chat" xmlns="jabber:client"><body>Hi Bob!</body></message>
>
```

### 6. Exit
Closes the program. All contacts are saved automatically.

---

## Testing Locally
1. Open two terminals.  
2. Start `ip_messenger` in each terminal with different ports.  
3. Add each other as contacts (use `localhost` 127.0.0.1 and the other user’s port).  
4. Send messages between the two terminals. Messages appear immediately in the recipient terminal.

---

## File Storage
Contacts are saved in `contacts.txt` with the format:

```text
Name IP Port
```

**Example:**

```text
Alice 127.0.0.1 5000
Bob 127.0.0.1 5001
```

---

## Makefile Commands

| Command       | Description              |
|---------------|-------------------------|
| `make`        | Compile ip_messenger     |
| `make clean`  | Remove compiled binary   |
