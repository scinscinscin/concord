#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h> /* SCNu64 */
#include <assert.h>

#include "discord.h"

void
print_usage(void)
{
    printf("\n\nThis bot demonstrates how easy it is to ban/unban members\n"
           "1. Type '!ban <user_id>' to ban user\n"
           "2. Type '!unban <user_id>' to unban user\n"
           "\nTYPE ANY KEY TO START BOT\n");
}

void
on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);

    log_info("Ban-Bot succesfully connected to Discord as %s#%s!",
             bot->username, bot->discriminator);
}

void
log_on_guild_ban_add(struct discord *client,
                     u64snowflake guild_id,
                     const struct discord_user *user)
{
    log_info("User `%s#%s` has been banned.", user->username,
             user->discriminator);
}

void
log_on_guild_ban_remove(struct discord *client,
                        u64snowflake guild_id,
                        const struct discord_user *user)
{
    log_info("User `%s#%s` has been unbanned.", user->username,
             user->discriminator);
}

void
on_ban(struct discord *client, const struct discord_message *msg)
{
    u64snowflake target_id = 0ULL;
    sscanf(msg->content, "%" SCNu64, &target_id);

    struct discord_create_guild_ban params = {
        .delete_message_days = 1,
        .reason = "Someone really dislikes you!",
    };
    discord_create_guild_ban(client, msg->guild_id, target_id, &params, NULL);
}

void
on_unban(struct discord *client, const struct discord_message *msg)
{
    u64snowflake target_id = 0ULL;
    sscanf(msg->content, "%" SCNu64, &target_id);

    discord_remove_guild_ban(client, msg->guild_id, target_id, NULL);
}

int
main(int argc, char *argv[])
{
    const char *config_file;
    if (argc > 1)
        config_file = argv[1];
    else
        config_file = "../config.json";

    ccord_global_init();
    struct discord *client = discord_config_init(config_file);
    assert(NULL != client && "Couldn't initialize client");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_guild_ban_add(client, &log_on_guild_ban_add);
    discord_set_on_guild_ban_remove(client, &log_on_guild_ban_remove);
    discord_set_on_command(client, "!ban", &on_ban);
    discord_set_on_command(client, "!unban", &on_unban);

    print_usage();
    fgetc(stdin); // wait for input

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();
}
