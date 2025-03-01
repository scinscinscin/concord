#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "discord.h"
#include "discord-internal.h"
#include "discord-request.h"

CCORDcode
discord_list_guild_emojis(struct discord *client,
                          u64snowflake guild_id,
                          struct discord_ret_emojis *ret)
{
    struct discord_request req = { 0 };

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");

    DISCORD_REQ_LIST_INIT(req, discord_emojis, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/guilds/%" PRIu64 "/emojis", guild_id);
}

CCORDcode
discord_get_guild_emoji(struct discord *client,
                        u64snowflake guild_id,
                        u64snowflake emoji_id,
                        struct discord_ret_emoji *ret)
{
    struct discord_request req = { 0 };

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, emoji_id != 0, CCORD_BAD_PARAMETER, "");

    DISCORD_REQ_INIT(req, discord_emoji, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/guilds/%" PRIu64 "/emojis/%" PRIu64, guild_id,
                               emoji_id);
}

CCORDcode
discord_create_guild_emoji(struct discord *client,
                           u64snowflake guild_id,
                           struct discord_create_guild_emoji *params,
                           struct discord_ret_emoji *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body;
    char buf[2048];

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, params != NULL, CCORD_BAD_PARAMETER, "");

    body.size = discord_create_guild_emoji_to_json(buf, sizeof(buf), params);
    body.start = buf;

    DISCORD_REQ_INIT(req, discord_emoji, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_POST,
                               "/guilds/%" PRIu64 "/emojis", guild_id);
}

CCORDcode
discord_modify_guild_emoji(struct discord *client,
                           u64snowflake guild_id,
                           u64snowflake emoji_id,
                           struct discord_modify_guild_emoji *params,
                           struct discord_ret_emoji *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body;
    char buf[2048];

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, emoji_id != 0, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, params != NULL, CCORD_BAD_PARAMETER, "");

    body.size = discord_modify_guild_emoji_to_json(buf, sizeof(buf), params);
    body.start = buf;

    DISCORD_REQ_INIT(req, discord_emoji, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_PATCH,
                               "/guilds/%" PRIu64 "/emojis/%" PRIu64, guild_id,
                               emoji_id);
}

CCORDcode
discord_delete_guild_emoji(struct discord *client,
                           u64snowflake guild_id,
                           u64snowflake emoji_id,
                           struct discord_ret *ret)
{
    struct discord_request req = { 0 };

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, emoji_id != 0, CCORD_BAD_PARAMETER, "");

    DISCORD_REQ_BLANK_INIT(req, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_DELETE,
                               "/guilds/%" PRIu64 "/emojis/%" PRIu64, guild_id,
                               emoji_id);
}
