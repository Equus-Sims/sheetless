Mutual Credit forms module (May 2011)

The intention of this module is to allow non-technical, or at least, non-drupal users the possibility to design and edit their own web forms in support of business rules and usability requirements.
Ctools is used to make these form definitions into exportable objects, which makes it easy to provide overridable defaults and to share them between sites. Integration with ctools was quite a messy affair, but I'm hoping Ctools APIs and documentation will improve with time.

Ideally, transactions would have a customisable workflow, in which, like documents through a large organisation, they move from state to state along a path in transitions performed by permitted users. There's a Drupal 6 module for this ‎which works on nodes but no sign in v7, where it would have to work on entities. This might be an interesting approach.

Each transaction form has a menu callback (which is of course unique) and produces an optional block. For each of the essential properties of the transaction, the widget can be configured. The widgets are then switched into an html template using tokens, so the themer can make the form look exactly as they want without touching php or editing template files on the server.

Instead of full workflow, each transaction form will edit transactions only in designated states, and will save the transaction in one given state, or unchanged. Further, the form itself has access control (via the menu callback), and the currency access controls apply as well.

Then there are 2 interesting enhancements:

1) Transactions are stored with the two user ids, i.e. from the perspective of a 3rd party, and the easiest way to make a transaction form is to enter the two parties directly.
However a more usable option is provided which assumes that the logged in user is initiating the transaction, so the unknown factors are then who is the other participant and which direction is the trade in. Some use-cases only allow trade in one direction and it's on profile pages a block can infer the other user from the url, which means the transaction form can be mostly pre-filled.

2) The 3rd party transactions have an option to allow multiple payers OR multiple payees.

