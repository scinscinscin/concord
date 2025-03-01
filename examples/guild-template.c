#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h> /* PRIu64, SCNu64 */
#include <assert.h>

#include "discord.h"

void
print_usage(void)
{
    printf("\n\nThis bot demonstrates how easy it is to manipulate guild"
           " template endpoints.\n"
           "1. Type 'guild-template.get <code>' to get a guild template's "
           "information\n"
           "2. Type 'guild-template.create' to create a new guild template\n"
           "3. Type 'guild-template.sync' to sync the guild template\n"
           "\nTYPE ANY KEY TO START BOT\n");
}

void
on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);

    log_info("Guild-Bot succesfully connected to Discord as %s#%s!",
             bot->username, bot->discriminator);
}

void
done(struct discord *client,
     void *data,
     const struct discord_guild_template *template)
{
    u64snowflake *channel_id = data;
    char text[DISCORD_MAX_MESSAGE_LEN];

    snprintf(text, sizeof(text),
             "Here is some information about your guild template:\nName: "
             "'%s'\nDescription: '%s'\nCreator Id: %" PRIu64 "\n",
             template->name, template->description, template->creator_id);

    struct discord_create_message params = { .content = text };
    discord_create_message(client, *channel_id, &params, NULL);
}

void
fail(struct discord *client, CCORDcode code, void *data)
{
    u64snowflake *channel_id = data;
    char text[DISCORD_MAX_MESSAGE_LEN];

    snprintf(text, sizeof(text), "Couldn't perform operation: %s",
             discord_strerror(code, client));

    struct discord_create_message params = { .content = text };
    discord_create_message(client, *channel_id, &params, NULL);
}

void
on_get_guild_template(struct discord *client,
                      const struct discord_message *msg)
{
    u64snowflake *channel_id = malloc(sizeof(u64snowflake));
    *channel_id = msg->channel_id;

    struct discord_ret_guild_template ret = {
        .done = &done,
        .fail = &fail,
        .data = channel_id,
        .cleanup = &free,
    };
    discord_get_guild_template(client, msg->content, &ret);
}

void
on_create_guild_template(struct discord *client,
                         const struct discord_message *msg)
{
    u64snowflake *channel_id = malloc(sizeof(u64snowflake));
    *channel_id = msg->channel_id;

    struct discord_ret_guild_template ret = {
        .done = &done,
        .fail = &fail,
        .data = channel_id,
        .cleanup = &free,
    };

    struct discord_create_guild_template params = {
        .name = "New server template!",
        .description = "This is a new server template created with Concord!"
    };

    discord_create_guild_template(client, msg->guild_id, &params, &ret);
}

void
on_sync_guild_template(struct discord *client,
                       const struct discord_message *msg)
{
    u64snowflake *channel_id = malloc(sizeof(u64snowflake));
    *channel_id = msg->channel_id;

    struct discord_ret_guild_template ret = {
        .done = &done,
        .fail = &fail,
        .data = channel_id,
        .cleanup = &free,
    };

    discord_sync_guild_template(client, msg->guild_id, msg->content, &ret);
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
    discord_set_prefix(client, "guild-template.");
    discord_set_on_command(client, "get", on_get_guild_template);
    discord_set_on_command(client, "create", on_create_guild_template);
    discord_set_on_command(client, "sync", on_sync_guild_template);

    print_usage();
    fgetc(stdin); // wait for input

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();
}
