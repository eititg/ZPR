#ifndef PACKET_HPP
#define PACKET_HPP

namespace net
{
    /** Typy ramek wymienianych między klientem i serwerem.
     */
    enum
    {
        PACKET_LOGIN_REQ,   /**< żądanie logowania*/
        PACKET_LOGIN_RES,   /**< odpowiedź na logowanie*/
        PACKET_LOGOUT_REQ,  /**< żądanie wylogowania*/
        PACKET_GET_GAME_LIST_REQ,   /**< żądanie pobrania listy gier*/
        PACKET_GAME_NAME_RES,   /**< odpowiedź z informcją o grze*/
        PACKET_GAME_DETAILS_REQ,    /**< żądanie pobrania szczegółowych informacji o grze*/
        PACKET_GAME_DETAILS_RES,    /**< odpowiedź ze szczegółowymi informacjami o grze*/
        PACKET_NEW_GAME_REQ,    /**< żądanie utworzenia nowej gry*/
        PACKET_NEW_GAME_RES,    /**< odpowiedź na żądanie utworzenia nowej gry*/
        PACKET_DELETE_GAME_REQ, /**< żądanie usunięcia gry*/
        PACKET_DELETE_GAME_RES, /**< odpowiedź na żądanie usunięcia gry*/
        PACKET_SUBSCRIBE_REQ,   /**< żądanie dołączenia do gry*/
        PACKET_SUBSCRIBE_RES,   /**< odpowiedź na żądanie dołączenia do gry*/
        PACKET_UNSUBSCRIBE_REQ, /**< żądanie opuszczcnia gry*/
        PACKET_UNSUBSCRIBE_RES, /**< odpowieź na żądanie opuszczenia gry*/
        PACKET_START_REQ    /**< żądanie rozpoczęcia gry*/
    };

    /** Kody statusu wykonania operacji.
     */
    enum
    {
        PACKET_YES, /**< powodzenie operacji*/
        PACKET_NO   /**< niepowodzenie operacji*/
    };
}

#endif // PACKET_HPP
