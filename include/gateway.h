/**
 * @file gateway.h
 * @author Cogmasters
 * @brief Gateway public functions and datatypes
 */

#ifndef DISCORD_GATEWAY_H
#define DISCORD_GATEWAY_H

/** @defgroup DiscordAPIGateway Gateway
 * @ingroup DiscordAPI
 * @brief Gateway's public API supported by Concord
 *  @{ */

/******************************************************************************
 * Custom functions
 ******************************************************************************/

/**
 * @brief Disconnect a member from voice channel
 *
 * @param client the client created with discord_init()
 * @param guild_id the guild the member belongs to
 * @param user_id the user to be disconnected
 * @CCORD_ret_obj{ret,guild_member}
 * @CCORD_return
 */
CCORDcode discord_disconnect_guild_member(
    struct discord *client,
    u64snowflake guild_id,
    u64snowflake user_id,
    struct discord_ret_guild_member *ret);

/******************************************************************************
 * Builder functions
 ******************************************************************************/

/**
 * @brief Helper function to add presence activities
 * @see discord_set_presence()
 *
 * This function is a wrapper over ntl_append2()
 */
void discord_presence_add_activity(struct discord_presence_update *presence,
                                   struct discord_activity *activity);

/******************************************************************************
 * Discord API functions
 ******************************************************************************/

/**
 * @brief Get a single valid WSS URL, which the client can use for connecting
 * @note This route should be cached, and only call the function again if
 *        unable to properly establishing a connection with the cached version
 * @warning This function blocks the running thread
 *
 * @param client the client created with discord_init()
 * @param ret if successful, a @ref sized_buffer containing the JSON response
 * @param ret a sized buffer containing the response JSON
 * @CCORD_return
 */
CCORDcode discord_get_gateway(struct discord *client,
                              struct sized_buffer *ret);

/**
 * @brief Get a single valid WSS URL, and additional metadata that can help
 *        during the operation of large bots.
 * @note This route should not be cached for extended periods of time as the
 *        value is not guaranteed to be the same per-call, and changes as the
 *        bot joins/leaves guilds
 * @warning This function blocks the running thread
 *
 * @param client the client created with discord_init()
 * @param ret if successful, a @ref sized_buffer containing the JSON response
 * @param ret a sized buffer containing the response JSON
 * @CCORD_return
 */
CCORDcode discord_get_gateway_bot(struct discord *client,
                                  struct sized_buffer *ret);

/** @} DiscordAPIGateway */

#endif /* DISCORD_GATEWAY_H */
