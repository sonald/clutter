#include <clutter/clutter.h>
#include "test-conform-common.h"

void
actor_add_child (TestConformSimpleFixture *fixture,
                 gconstpointer dummy)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "baz",
                                                NULL));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 3);

  iter = clutter_actor_get_first_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");

  iter = clutter_actor_get_next_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  iter = clutter_actor_get_next_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");
  g_assert (iter == clutter_actor_get_last_child (actor));
  g_assert (clutter_actor_get_next_sibling (iter) == NULL);

  iter = clutter_actor_get_last_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");

  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");
  g_assert (iter == clutter_actor_get_first_child (actor));
  g_assert (clutter_actor_get_previous_sibling (iter) == NULL);

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_insert_child (TestConformSimpleFixture *fixture,
                    gconstpointer data)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_insert_child_at_index (actor,
                                       g_object_new (CLUTTER_TYPE_ACTOR,
                                                     "name", "foo",
                                                     NULL),
                                       0);

  iter = clutter_actor_get_first_child (actor);
  g_assert (iter != NULL);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");
  g_assert (iter == clutter_actor_get_child_at_index (actor, 0));

  clutter_actor_insert_child_below (actor,
                                    g_object_new (CLUTTER_TYPE_ACTOR,
                                                  "name", "bar",
                                                  NULL),
                                    iter);

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 2);

  iter = clutter_actor_get_first_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");
  iter = clutter_actor_get_next_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");
  g_assert (iter == clutter_actor_get_child_at_index (actor, 1));

  iter = clutter_actor_get_first_child (actor);
  clutter_actor_insert_child_above (actor,
                                    g_object_new (CLUTTER_TYPE_ACTOR,
                                                  "name", "baz",
                                                  NULL),
                                    iter);

  iter = clutter_actor_get_last_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");

  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");

  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_remove_child (TestConformSimpleFixture *fixture,
                    gconstpointer data)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 2);

  g_assert (clutter_actor_get_first_child (actor) != clutter_actor_get_last_child (actor));

  iter = clutter_actor_get_first_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");

  iter = clutter_actor_get_last_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  clutter_actor_remove_child (actor, clutter_actor_get_first_child (actor));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 1);

  iter = clutter_actor_get_first_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");
  g_assert (clutter_actor_get_first_child (actor) == clutter_actor_get_last_child (actor));

  clutter_actor_remove_child (actor, clutter_actor_get_first_child (actor));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 0);
  g_assert (clutter_actor_get_first_child (actor) == NULL);
  g_assert (clutter_actor_get_last_child (actor) == NULL);

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_raise_child (TestConformSimpleFixture *fixture,
                   gconstpointer dummy)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "baz",
                                                NULL));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 3);

  iter = clutter_actor_get_child_at_index (actor, 1);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  clutter_actor_raise (iter, clutter_actor_get_child_at_index (actor, 2));

  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 0)),
                   ==,
                   "foo");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 1)),
                   ==,
                   "baz");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 2)),
                   ==,
                   "bar");

  iter = clutter_actor_get_child_at_index (actor, 0);
  clutter_actor_raise_top (iter);

  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 0)),
                   ==,
                   "baz");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 1)),
                   ==,
                   "bar");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 2)),
                   ==,
                   "foo");

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_lower_child (TestConformSimpleFixture *fixture,
                   gconstpointer dummy)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "baz",
                                                NULL));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 3);

  iter = clutter_actor_get_child_at_index (actor, 1);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  clutter_actor_lower (iter, clutter_actor_get_child_at_index (actor, 0));

  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 0)),
                   ==,
                   "bar");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 1)),
                   ==,
                   "foo");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 2)),
                   ==,
                   "baz");

  iter = clutter_actor_get_child_at_index (actor, 2);
  clutter_actor_lower_bottom (iter);

  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 0)),
                   ==,
                   "baz");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 1)),
                   ==,
                   "bar");
  g_assert_cmpstr (clutter_actor_get_name (clutter_actor_get_child_at_index (actor, 2)),
                   ==,
                   "foo");

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_replace_child (TestConformSimpleFixture *fixture,
                     gconstpointer dummy)
{
  ClutterActor *actor = clutter_actor_new ();
  ClutterActor *iter;

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));

  iter = clutter_actor_get_child_at_index (actor, 0);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");

  clutter_actor_replace_child (actor, iter,
                               g_object_new (CLUTTER_TYPE_ACTOR,
                                             "name", "baz",
                                             NULL));

  iter = clutter_actor_get_child_at_index (actor, 0);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");

  iter = clutter_actor_get_child_at_index (actor, 1);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");

  clutter_actor_replace_child (actor, iter,
                               g_object_new (CLUTTER_TYPE_ACTOR,
                                             "name", "qux",
                                             NULL));

  iter = clutter_actor_get_child_at_index (actor, 0);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");

  iter = clutter_actor_get_child_at_index (actor, 1);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "qux");

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo"));

  clutter_actor_replace_child (actor, iter,
                               g_object_new (CLUTTER_TYPE_ACTOR,
                                             "name", "bar",
                                             NULL));

  iter = clutter_actor_get_last_child (actor);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "foo");
  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "bar");
  iter = clutter_actor_get_previous_sibling (iter);
  g_assert_cmpstr (clutter_actor_get_name (iter), ==, "baz");

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}

void
actor_remove_all (TestConformSimpleFixture *fixture,
                  gconstpointer dummy)
{
  ClutterActor *actor = clutter_actor_new ();

  g_object_ref_sink (actor);

  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "foo",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "bar",
                                                NULL));
  clutter_actor_add_child (actor, g_object_new (CLUTTER_TYPE_ACTOR,
                                                "name", "baz",
                                                NULL));

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 3);

  clutter_actor_remove_all_children (actor);

  g_assert_cmpint (clutter_actor_get_n_children (actor), ==, 0);

  clutter_actor_destroy (actor);
  g_object_unref (actor);
}