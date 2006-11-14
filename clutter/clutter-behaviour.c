/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Authored By Matthew Allum  <mallum@openedhand.com>
 *
 * Copyright (C) 2006 OpenedHand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:clutter-behaviour
 * @short_description: Class for providing behaviours to actors
 *
 */

#include "config.h"

#include "clutter-actor.h"
#include "clutter-behaviour.h"

G_DEFINE_ABSTRACT_TYPE (ClutterBehaviour,
                        clutter_behaviour,
                        G_TYPE_OBJECT);

struct _ClutterBehaviourPrivate
{
  ClutterAlpha *alpha;

  guint notify_id;
  GSList *actors;
};

enum
{
  PROP_0,
  PROP_ALPHA
};

enum {
  SIGNAL_LAST
};


#define CLUTTER_BEHAVIOUR_GET_PRIVATE(obj)         \
              (G_TYPE_INSTANCE_GET_PRIVATE ((obj), \
               CLUTTER_TYPE_BEHAVIOUR,             \
               ClutterBehaviourPrivate))

static void 
clutter_behaviour_finalize (GObject *object)
{
  ClutterBehaviour *self = CLUTTER_BEHAVIOUR (object);

  clutter_behaviour_set_alpha (self, NULL);
  
  g_slist_foreach (self->priv->actors, (GFunc) g_object_unref, NULL);
  g_slist_free (self->priv->actors);

  G_OBJECT_CLASS (clutter_behaviour_parent_class)->finalize (object);
}

static void
clutter_behaviour_set_property (GObject      *object, 
				guint         prop_id,
				const GValue *value, 
				GParamSpec   *pspec)
{
  ClutterBehaviour *behaviour;

  behaviour = CLUTTER_BEHAVIOUR(object);

  switch (prop_id) 
    {
    case PROP_ALPHA:
      clutter_behaviour_set_alpha (behaviour, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
clutter_behaviour_get_property (GObject    *object, 
			        guint       prop_id,
			        GValue     *value, 
			        GParamSpec *pspec)
{
  ClutterBehaviour        *behaviour;
  ClutterBehaviourPrivate *priv;

  behaviour = CLUTTER_BEHAVIOUR(object);
  priv      = CLUTTER_BEHAVIOUR_GET_PRIVATE(behaviour);

  switch (prop_id) 
    {
    case PROP_ALPHA:
      g_value_set_object (value, priv->alpha);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    } 
}


static void
clutter_behaviour_class_init (ClutterBehaviourClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize     = clutter_behaviour_finalize;
  object_class->set_property = clutter_behaviour_set_property;
  object_class->get_property = clutter_behaviour_get_property;

  g_object_class_install_property (object_class,
                                   PROP_ALPHA,
                                   g_param_spec_object ("alpha",
                                                        "Alpha",
                                                        "Alpha Object to drive the behaviour",
                                                        CLUTTER_TYPE_ALPHA,
                                                        G_PARAM_CONSTRUCT |
                                                        G_PARAM_READWRITE));

  g_type_class_add_private (klass, sizeof (ClutterBehaviourPrivate));
}

static void
clutter_behaviour_init (ClutterBehaviour *self)
{
  ClutterBehaviourPrivate *priv;

  self->priv = priv = CLUTTER_BEHAVIOUR_GET_PRIVATE (self);

}

void
clutter_behaviour_apply (ClutterBehaviour *behave,
                         ClutterActor     *actor)
{
  g_return_if_fail (CLUTTER_IS_BEHAVIOUR (behave));
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  if (g_slist_find (behave->priv->actors, actor))
    {
      g_warning ("The behaviour of type %s already applies "
                 "to the actor of type %s",
                 g_type_name (G_OBJECT_TYPE (behave)),
                 g_type_name (G_OBJECT_TYPE (actor)));
      return;
    }

  g_object_ref (actor);
  behave->priv->actors = g_slist_prepend (behave->priv->actors, actor);
}

void
clutter_behaviour_remove (ClutterBehaviour *behave,
                          ClutterActor     *actor)
{
  g_return_if_fail (CLUTTER_IS_BEHAVIOUR (behave));
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  if (!g_slist_find (behave->priv->actors, actor))
    {
      g_warning ("The behaviour of type %s does not apply "
                 "to the actor of type %s",
                 g_type_name (G_OBJECT_TYPE (behave)),
                 g_type_name (G_OBJECT_TYPE (actor)));
      return;
    }
  
  g_object_unref (actor);
  behave->priv->actors = g_slist_remove (behave->priv->actors, actor);
}

void
clutter_behaviour_actors_foreach (ClutterBehaviour *behave,
				  GFunc             func,
				  gpointer          userdata)
{
  g_return_if_fail (CLUTTER_IS_BEHAVIOUR (behave));
  g_return_if_fail (func != NULL);

  g_slist_foreach (behave->priv->actors, func, userdata);
}

ClutterAlpha*
clutter_behaviour_get_alpha (ClutterBehaviour *behave)
{
  g_return_val_if_fail (CLUTTER_IS_BEHAVIOUR (behave), NULL);

  return behave->priv->alpha;
}

static void
notify_cb (GObject          *object,
           GParamSpec       *param_spec,
           ClutterBehaviour *behave)
{
  ClutterBehaviourClass *class;

  class = CLUTTER_BEHAVIOUR_GET_CLASS(behave);

  if (class->alpha_notify)
    class->alpha_notify (behave);
}

void
clutter_behaviour_set_alpha (ClutterBehaviour *behave,
			     ClutterAlpha     *alpha)
{
  ClutterBehaviourPrivate *priv;

  g_return_if_fail (CLUTTER_IS_BEHAVIOUR (behave));
  g_return_if_fail (CLUTTER_IS_ALPHA (alpha));

  priv = behave->priv;

  if (priv->notify_id)
    {
      g_signal_handler_disconnect (priv->alpha, priv->notify_id);
      priv->notify_id = 0;
    }

  if (priv->alpha)
    {
      g_object_unref (priv->alpha);
      priv->alpha = NULL;
    }

  if (alpha)
    {
      priv->alpha = alpha;
      g_object_ref_sink (priv->alpha);

      priv->notify_id = g_signal_connect (priv->alpha, "notify::alpha",
                                          G_CALLBACK(notify_cb),
                                          behave);
    }
}
