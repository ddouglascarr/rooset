ScheduledNotificationToSend = mondelefant.new_class()
ScheduledNotificationToSend.table = 'scheduled_notification_to_send'

function ScheduledNotificationToSend:get_next()
  return self:new_selector()
    :add_order_by("pending DESC")
    :add_order_by("random()")
    :limit(1)
    :optional_object_mode()
    :exec()
end