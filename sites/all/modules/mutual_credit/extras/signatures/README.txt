
Pending module.

This module is controlled entirely through the actions / trigger framework, and can be more finely tweaked using Rules API.
Two actions and two triggers are provided, and the default 'Notify transactees is altered to become 'Mail all signatories';
Some simple defaults are provided
When a new transaction is inserted:
 * Add signatories to new transactions of type '1stparty' (defined in the mcapi_forms module);
 * Send a mail to the signatories (You will want to configure this mail)
Remember you can use the same action more than once

It is possible for a user to sign all their transactions at once.
And administators can 'sign off' a transaction which means signing for everybody.

The module declares a new state, TRAANSACTION_STATE_PENDING which doesn't count in the standard balance calculation.
It fires a trigger whenever the transaction is signed
And another trigger when the transaction moves from pending to finished states.

Views integrationi used to be provided, but has probably been removed.
Its very tricky working with the linked signatures table to provide all the likely permutations.
Instead callbacks have been provided which show the sentence, or 'tokens' display mode.
