Story:

Structurally, the load balancer is comprised of a circular doubly linked list,
which represents the hashring. The servers will be stored on this hashring
and each of them will contain an ID and a hashtable. Items, key-value
pairs, will be stored on those hashtables.

For efficiency, each server whose ID is less than 10^5, will have 2 replicas.
The first replica's ID is 10^5 + the original server's ID, and the second's
is 2 * 10^5 + the original ID. However, these 2 replicas will not have a
hashtable of their own. Instead, their hashtable will be a pointer to the
original server's hashtable.

The servers, including their replicas, are added in the hashring in ascending
order by their hashed ID. Similarly, an item will be stored on the closest
server with a bigger hashed ID than its hashed key. Not all items are loyal
though. Why? Because whenever a new server with a "prettier" hashed ID is
added in the hashring, items can migrate to it if their hashed key says so.

When a server is removed from the hashring, both its replicas are removed as
well and thus, item migration is compulsory. All items that belonged to the
removed server will now be stored on its neighbour (removed_server->next
in the hashring).

Code:

- the implementations for the linked list, cdll and hashtable are the ones
used for solving previous data structure labs
- the hashring files manage how servers are added, searched or removed in and
from the circular list; the reassign function handles the item migration
mentioned above
- more elaborate comments about what each function does can be found in the
header files
